require_relative 'support/spec_helper'
require 'timeout'
require 'socket'
require 'tmpdir'

require_relative '../lib/acpc_dealer'
require_relative '../lib/acpc_dealer/dealer_runner'
include AcpcDealer

describe DealerRunner do
  let(:various_numbers_of_players) { [2, 3] }

  def cleanup(pid)
    Process.kill 'KILL', pid
    begin
      Timeout::timeout(3) do
        Process.getpgid(pid)
        sleep 0.2
      end
    rescue Errno::ESRCH
    else
      ->() { Process.getpgid(pid) }.must_raise Errno::ESRCH
    end
  end

  def check_process_exists(pid)
    Process.getpgid(pid)
  rescue Errno::ESRCH => e
    flunk "Dealer with pid #{pid} does not exist: #{e.message}"
  end

  def check_ports_are_in_use(port_numbers)
    port_numbers.each do |port|
      begin
        Socket.tcp('localhost', port, connect_timeout: 1)
      rescue
        flunk "No dealer listening on port #{port}"
      end
    end
  end

  describe '::ports_for_players' do
    it 'returns two random open ports' do
      various_numbers_of_players.each do |number_of_players|
        DealerRunner.ports_for_players(number_of_players).each do |port|
          begin
            TCPServer.open('localhost', port) {}
          rescue Errno::EADDRINUSE => e
            flunk "Port #{port} is not open: #{e.message}"
          end
        end
      end
    end
  end

  def in_tmp_dir_with_process_cleanup
    Dir.mktmpdir do |temp_log_directory|
      cleanup(yield(temp_log_directory))
    end
  end

  describe '::start' do
    let(:options) { ['--start_timeout 1000'].join(' ') }
    let(:match_name) { 'test_match' }
    let(:hands) { 10 }
    let(:random_seed) { 0 }

    def player_names(number_of_players)
      number_of_players.times.inject([]) do |names, i|
        names << "p#{i}"
      end.join(' ')
    end

    def game_def(number_of_players)
      GAME_DEFINITION_FILE_PATHS[number_of_players][:limit]
    end

    it 'starts a dealer asynchronously that can clean up after itself and returns both its PID and the ports that each player can use to connect' do
      various_numbers_of_players.each do |number_of_players|
        in_tmp_dir_with_process_cleanup do |temp_log_directory|
          patient = DealerRunner.start(
            {
              match_name: match_name,
              game_def_file_name: game_def(number_of_players),
              hands: hands,
              random_seed: random_seed,
              player_names: player_names(number_of_players),
              options: options
            },
            temp_log_directory
          )

          patient[:log_directory].must_equal temp_log_directory
          check_process_exists patient[:pid]
          check_ports_are_in_use patient[:port_numbers]
          patient[:pid]
        end
      end
    end

    it 'starts a dealer listening on specific ports' do
      various_numbers_of_players.each do |number_of_players|
        in_tmp_dir_with_process_cleanup do |temp_log_directory|
          port_numbers = [10_000, 20_000]
          patient = DealerRunner.start(
            {
              match_name: match_name,
              game_def_file_name: game_def(number_of_players),
              hands: hands,
              random_seed: random_seed,
              player_names: player_names(number_of_players),
              options: options
            },
            temp_log_directory,
            port_numbers
          )

          patient[:port_numbers].map(&:to_i)[0..port_numbers.length - 1].must_equal port_numbers

          patient[:log_directory].must_equal temp_log_directory
          check_process_exists patient[:pid]
          check_ports_are_in_use patient[:port_numbers]
          patient[:pid]
        end
      end
    end
  end
end
