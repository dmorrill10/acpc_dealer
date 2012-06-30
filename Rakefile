#!/usr/bin/env rake

require "bundler/gem_tasks"
require 'rake'
require 'rake/extensiontask'
require 'rspec/core/rake_task'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

require 'fileutils'

Rake::ExtensionTask.new('hand_evaluator') do |t|
  FileUtils.cd File.expand_path('../ext/project_acpc_server', __FILE__)
  sh %{ make }
  FileUtils.cd File.expand_path('..', __FILE__)
end

RSpec::Core::RakeTask.new(:spec) do |t|
  ruby_opts = "-w"
end

desc 'Compile, build, tag, and run specs'
task :default => :compile do
  Rake::Task[:spec].invoke
  Rake::Task[:tag].invoke
end

task :build => :compile do
  Rake::Task[:spec].invoke
  system "gem build acpc_poker_types.gemspec"
end

task :tag => :build do
  puts "Tagging #{AcpcDealer::VERSION}..."
  system "git tag -a #{AcpcDealer::VERSION} -m 'Tagging #{AcpcDealer::VERSION}'"
  puts "Pushing #{AcpcDealer::VERSION} to git..."
  system "git push --tags"
end

task :install => :compile do
end
