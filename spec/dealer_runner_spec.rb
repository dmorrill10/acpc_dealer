require_relative 'support/spec_helper'

require 'socket'
require 'tmpdir'

require_relative '../lib/acpc_dealer'
require_relative '../lib/acpc_dealer/dealer_runner'
include AcpcDealer

describe DealerRunner do
  before do
    reset_state!
  end

  def reset_state!
    @pid = nil
    @port_numbers = nil
  end

  describe '::ports_for_players' do
    it 'returns two random open ports' do
      various_numbers_of_players.each do |number_of_players|
        DealerRunner.ports_for_players(number_of_players).each do |port|
          begin
            TCPServer.open('localhost', port) { }
          rescue Errno::EADDRINUSE => e
            flunk "Port #{port} is not open: #{e.message}"
          end
        end
      end
    end
  end

  describe '::start' do
    it 'starts a dealer asynchronously that can clean up after itself and returns both its PID and the ports that each player can use to connect' do
      various_numbers_of_players.each do |number_of_players|
        Dir.mktmpdir do |temp_log_directory|
          result = DealerRunner.start(
            {
              match_name: 'test_match',
              game_def_file_name: GAME_DEFINITION_FILE_PATHS[number_of_players][:limit],
              hands: 10,
              random_seed: 0,
              player_names: number_of_players.times.inject([]) do |names, i|
                names << "p#{i}"
              end.join(' '),
              options: ['--start_timeout 1000'].join(' ')
            },
            temp_log_directory
          )

          @pid = result[:pid]
          @port_numbers = result[:port_numbers]

          result[:log_directory].must_equal temp_log_directory

          check_ports_are_in_use

          check_process_exists

          cleanup
          reset_state!
        end
      end
    end

    it 'starts a dealer listening on specific ports' do
      various_numbers_of_players.each do |number_of_players|
        Dir.mktmpdir do |temp_log_directory|
          port_numbers = [10000, 20000]
          result = DealerRunner.start(
            {
              match_name: 'test_match',
              game_def_file_name: GAME_DEFINITION_FILE_PATHS[number_of_players][:limit],
              hands: 10,
              random_seed: 0,
              player_names: number_of_players.times.inject([]) do |names, i|
                names << "p#{i}"
              end.join(' '),
              options: ['--start_timeout 1000'].join(' ')
            },
            temp_log_directory,
            port_numbers
          )

          @pid = result[:pid]
          @port_numbers = result[:port_numbers]
          (@port_numbers.map { |n| n.to_i })[0..port_numbers.length-1].must_equal port_numbers

          result[:log_directory].must_equal temp_log_directory

          check_ports_are_in_use

          check_process_exists

          cleanup
          reset_state!
        end
      end
    end
  end

  def cleanup
    Process.kill 'KILL', @pid
    sleep 1

    ->() { Process.getpgid(@pid) }.must_raise Errno::ESRCH
  end
  def check_process_exists
    begin
      Process.getpgid(@pid)
    rescue Errno::ESRCH => e
      flunk "Dealer with pid #{@pid} does not exist: #{e.message}"
    end
  end
  def check_ports_are_in_use
    @port_numbers.each do |port|
      ->() do
        TCPServer.open('localhost', port)
      end.must_raise Errno::EADDRINUSE
    end
  end
  def various_numbers_of_players() [2, 3] end
end
