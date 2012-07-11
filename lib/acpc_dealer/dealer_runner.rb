
require 'socket'

require 'dmorrill10-utils/process_runner'
require File.expand_path('../../acpc_dealer', __FILE__)

class DealerRunner

  # @return [Array<Symbol>] The format of the dealer command. The symbols in this
  # => array should correspond to those in the +dealer_arguments+ of ::start.
  # => (see ::start)
  DEALER_COMMAND_FORMAT = [
    # @note The name of the match to start.
    :match_name, 
    # @note The path of the game definition to use.
    :game_def_file_name, 
    # @note The number of hands to play.
    :hands,
    # @note The random seed to use.
    :random_seed, 
    # @note The names of the players in the game. Should be specified as a space delimited string of names.
    :player_names,
    # @note Should be specified as an option string that the dealer will understand.
    :options 
  ]

  # @param [Array] dealer_arguments Arguments to the new dealer instance.
  # @param [String] log_directory The directory in which logs will be placed.
  # => Defaults to +<dealer_arguments[:match_name]>.logs+.
  # @param [Array] port_numbers The port numbers to which each player will connect.
  # => Defaults to random.
  # @return [Hash] The process ID of the started dealer (key +:pid+) and the array of ports that players may 
  # => use to connect to the new dealer instance (key +:port_numbers+).
  # @raise (see ProcessRunner::go)
  # @raise (see FileUtils::mkdir_p)
  def self.start(dealer_arguments, log_directory=nil, port_numbers=nil)
    dealer_start_command = DEALER_COMMAND_FORMAT.inject([AcpcDealer::DEALER_PATH]) do |command, parameter|
      command << dealer_arguments[parameter].to_s
    end 
    dealer_start_command << "-p #{port_numbers.join(',')}" if port_numbers

    unless log_directory
      log_directory = File.expand_path("../#{dealer_arguments[:match_name]}.logs", __FILE__)
    end

    FileUtils.mkdir_p log_directory

    IO.pipe do |read_io, write_io|
      pid = ProcessRunner.go(
        dealer_start_command, 
        err: [
          "#{dealer_arguments[:match_name]}.actions.log", 
          File::CREAT|File::WRONLY
        ],
        out: write_io,
        chdir: log_directory
      )

      {pid: pid, port_numbers: read_io.gets.split, log_directory: log_directory}
    end
  end

  # @todo How could ports be reserved without starting a server? Would +::ports_for_players+ even want to do that?

  # @return [Array<Integer>] List of random open ports. Does NOT reserve the ports though,
  # => so it's possible that the ports will come into use between calling this method and
  # => using them.
  def self.ports_for_players(number_of_players)
    number_of_players.times.inject([]) do |ports, i|
      ports << TCPServer.open('localhost', 0) { |s| s.addr[1] }
    end
  end
end
