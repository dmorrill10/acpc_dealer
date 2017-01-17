require 'socket'

module AcpcDealer
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

    Arguments = Struct.new(*DEALER_COMMAND_FORMAT)

    # @param [Array] dealer_arguments Arguments to the new dealer instance.
    # @param [Array] port_numbers The port numbers to which each player will connect.
    # => Defaults to the ACPC Dealer's default.
    # @return [Array] The components of the ACPC Dealer command that would be run (the executable, followed by arguments)
    def self.command_components(dealer_arguments, port_numbers=nil)
      dealer_start_command = DEALER_COMMAND_FORMAT.inject(
        [AcpcDealer::DEALER_PATH]
      ) do |command_, parameter|
        command_ += dealer_arguments[parameter].to_s.split
      end
      if port_numbers
        dealer_start_command << "-p" << "#{port_numbers.join(',')}"
      end
      dealer_start_command
    end

    # @param [Array] dealer_arguments Arguments to the new dealer instance.
    # @param [Array] port_numbers The port numbers to which each player will connect.
    # => Defaults to the ACPC Dealer's default.
    # @return [String] The ACPC Dealer command that would be run
    def self.command(dealer_arguments, port_numbers=nil)
      command_components(dealer_arguments, port_numbers).join(' ')
    end

    # @param [Array] dealer_arguments Arguments to the new dealer instance.
    # @param [String] log_directory The directory in which logs will be placed.
    # => Defaults to +<dealer_arguments[:match_name]>.logs+.
    # @param [Array] port_numbers The port numbers to which each player will connect.
    # => Defaults to the ACPC's default.
    # @param [Boolean] save_action_log
    # @param [Hash] options (see Process::spawn)
    # @raise (see Process::detach)
    # @raise (see Process::spawn)
    # @raise (see FileUtils::mkdir_p)
    def self.start(
      dealer_arguments,
      log_directory=nil,
      port_numbers=nil,
      save_action_log=true,
      options={}
    )
      log_directory ||= File.expand_path(
        "../#{dealer_arguments[:match_name]}.logs",
        __FILE__
      )
      FileUtils.mkdir_p log_directory unless Dir.exist?(log_directory)

      options[:chdir] ||= log_directory

      IO.pipe do |read_io, write_io|
        options[:out] ||= write_io
        if save_action_log
          options[:err] = [
            File.join(
              log_directory,
              "#{dealer_arguments[:match_name]}.actions.log"
            ),
            File::CREAT|File::WRONLY
          ]
        end
        pid = Process.detach(
          Process.spawn(
            *command_components(dealer_arguments, port_numbers),
            options
          )
        ).pid

        {
          pid: pid,
          port_numbers: read_io.gets.split.map(&:to_i),
          log_directory: log_directory
        }
      end
    end

    # @return [Array<Integer>] List of arbitrary open ports.
    # => Does NOT reserve the ports,
    # => so it's possible that the ports will come into use between calling
    # => this method and using them.
    def self.ports_for_players(number_of_players)
      number_of_players.times.map do
        TCPServer.open('localhost', 0) { |s| s.addr[1] }
      end
    end
  end
end
