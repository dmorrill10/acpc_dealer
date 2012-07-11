require "bundler/gem_tasks"
require 'rake'
require 'rake/extensiontask'
require 'rake/testtask'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

Rake::ExtensionTask.new('hand_evaluator')

desc 'Build gem'
task :default => :build

task :build => :compile

Rake::TestTask.new do |t|
  t.libs << "lib" << 'spec/support'
  t.test_files = FileList['spec/**/*_spec.rb']
  t.verbose = true
  t.warning = true
end
