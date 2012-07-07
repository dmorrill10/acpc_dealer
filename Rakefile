require "bundler/gem_tasks"
require 'rake'
require 'rake/extensiontask'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

Rake::ExtensionTask.new('hand_evaluator')

desc 'Build gem'
task :default => :build

task :build => :compile

task :tag => :build do
  puts "Tagging #{AcpcDealer::VERSION}..."
  system "git tag -a #{AcpcDealer::VERSION} -m 'Tagging #{AcpcDealer::VERSION}'"
  puts "Pushing #{AcpcDealer::VERSION} to git..."
  system "git push --tags"
end
