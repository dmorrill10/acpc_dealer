
require 'dmorrill10-utils/process_runner'
require File.expand_path('../../acpc_dealer', __FILE__)

class DealerRunner
  # @param [String] match_name The name of the match to start.
  # @param [Array] dealer_arguments Arguments to the new dealer instance.
  # @param [String] log_directory The directory in which logs will be placed.
  # @raise (see ProcessRunner#go)
  def self.start(match_name, dealer_arguments, log_directory)
    dealer_start_command = dealer_arguments.unshift(match_name).unshift(AcpcDealer::DEALER_PATH)

    FileUtils.mkdir_p log_directory

    ProcessRunner.go(
      dealer_start_command, 
      err: ["#{match_name}.actions.log", File::CREAT|File::WRONLY],
      chdir: log_directory
    )
  end
end
