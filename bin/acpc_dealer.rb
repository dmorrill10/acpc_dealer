#!/usr/bin/env ruby

require 'dmorrill10-utils/script'

require 'fileutils'

require 'clive'

require File.expand_path('../../lib/acpc_dealer', __FILE__)

class AcpcDealerCli < Clive

  def self.print_usage(command_symbol)
    puts AcpcDealerCli.find_command(command_symbol).help
    exit
  end

  def self.game_def_from_components(number_of_players, betting_type)
    begin
      game_def = AcpcDealer::GAME_DEFINITION_FILE_PATHS[number_of_players][betting_type]
      raise unless game_def
    rescue
      puts "Unable to find game definition for #{number_of_players} players with betting type '#{betting_type}'"
      puts "Available game definitions:"
      AcpcDealer::GAME_DEFINITION_FILE_PATHS.each do |num_players, game_defs|
        game_defs.each do |type, path|
          puts "    #{num_players} #{type}"
        end
      end
      exit
    end

    game_def
  end

  command :compile, 'Compiles the ACPC Dealer program' do
    opt :c, :clean, 'Do a clean and full recompile' do
      print 'Cleaning...'
      FileUtils.cd AcpcDealer::DEALER_DIRECTORY do
        system "make clean"
      end
      puts 'Done'
    end

    action do
      print 'Compiling...'
      FileUtils.cd AcpcDealer::DEALER_DIRECTORY do
        system "make"
      end
      puts 'Done'
    end
  end

  command :dealer, 'Runs the ACPC Dealer' do
    desc 'The name of the match, defaults to <number_of_players>p.<betting_type>.h<hands>.r<random_seed>'
    opt :m, :name, arg: '<name>', as: String
    
    desc 'Parameters that determine which game definition to use'
    opt :g, :game_def, args: '<number_of_players> <betting_type>', as: [Integer, Symbol]
    
    opt :n, :hands, 'The number of hands to play', arg: '<hands>', as: Integer
    
    desc 'The random seed to determine how the cards are dealt, defaults to a random random seed'
    opt :r, :random_seed, arg: '<random_seed>', as: Integer
    
    desc 'The names of the players participating in the match, defaults to "p1", "p2", ...'
    opt :p, :player_names, args: '<p1_name>...', as: [String]

    bool :f, :fixed_button, 'Use fixed dealer button at table'
    
    opt :l, :log_directory, 'Directory where logs will be placed'

    opt :t_response, 'Maximum time per response', arg: '<t_response_milliseconds>', as: Integer
    
    opt :t_hand, 'Maximum player time per hand', arg: '<t_hand_milliseconds>', as: Integer
    
    opt :t_per_hand, 'Maximum average player time for match', arg: '<t_per_hand_milliseconds>', as: Integer 
    
    desc 'maximum time to wait for players to connect, defaults to no timeout'
    opt :start_timeout, arg: '<start_timeout_milliseconds>', as: Integer

    action do
      AcpcDealerCli.print_usage(:dealer) unless get(:game_def)
      AcpcDealerCli.print_usage(:dealer) unless get(:hands)

      (number_of_players, betting_type) = get(:game_def)
      game_def = AcpcDealerCli.game_def_from_components(number_of_players, betting_type)

      random_seed = if get(:random_seed)
        get(:random_seed)
      else
        random_float = rand
        (random_float * 10**random_float.to_s.length).to_i
      end

      name = if get(:name)
        get(:name) 
      else 
        "#{number_of_players}p.#{betting_type}.h#{get(:hands)}.r#{random_seed}"
      end

      player_names = if get(:player_names) 
        get(:player_names)
      else
        []
      end
      
      while player_names.length < get(:game_def).first
          player_names << "p#{player_names.length+1}"
      end

      arguments = [game_def, get(:hands).to_s, random_seed.to_s] + player_names

      arguments << '-f' if get(:f)
      arguments << "--t_response #{get(:t_response_milliseconds)}" if get(:t_response_milliseconds)
      arguments << "--t_hand #{get(:t_hand_milliseconds)}" if get(:t_hand_milliseconds)
      arguments << "--t_per_hand #{get(:t_per_hand_milliseconds)}" if get(:t_per_hand_milliseconds)
      arguments << "--start_timeout #{get(:start_timeout_milliseconds)}" if get(:start_timeout_milliseconds)

      log_directory = if get(:log_directory)
        get(:log_directory)
      else
        File.join(FileUtils.pwd, 'dealer_logs')
      end

      AcpcDealerCli.run(['compile']) unless File.exists? AcpcDealer::DEALER_PATH

      puts "Ports: #{DealerRunner.start(name, arguments, log_directory).gets}"
    end
  end
end

if $0 == __FILE__
  if ARGV.empty?
    puts AcpcDealerCli.help
  else  
    AcpcDealerCli.run
  end
end
