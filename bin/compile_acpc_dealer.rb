#!/usr/bin/env ruby

require 'fileutils'

require 'acpc_dealer'

FileUtils.cd AcpcDealer::DEALER_DIRECTORY do
  system "make"
end

RUBY_BIN = "#{ENV['GEM_HOME']}/bin/"
unless File.exists? "#{RUBY_BIN}/dealer"
  FileUtils.symlink AcpcDealer::DEALER_PATH, "#{ENV['GEM_HOME']}/bin/"
end
