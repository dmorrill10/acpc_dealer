require "bundler/gem_tasks"
require 'rake'
require 'rake/extensiontask'
require 'rspec/core/rake_task'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

require 'fileutils'

Rake::ExtensionTask.new('hand_evaluator')

RSpec::Core::RakeTask.new(:spec) do |t|
  ruby_opts = "-w"
end

desc 'Build gem'
task :default => :build

task :build => [:compile, :spec]

task :tag => :build do
  puts "Tagging #{AcpcDealer::VERSION}..."
  system "git tag -a #{AcpcDealer::VERSION} -m 'Tagging #{AcpcDealer::VERSION}'"
  puts "Pushing #{AcpcDealer::VERSION} to git..."
  system "git push --tags"
end

Rake::Task[:compile].prerequisites.unshift('vendor/project_acpc_server/dealer')
