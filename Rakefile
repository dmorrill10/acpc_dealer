require "bundler/gem_tasks"
require 'rake'
require 'rake/clean'
require 'rbconfig'
require 'rake/testtask'
require 'rake/extensiontask'
require 'rubygems/package_task'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

desc 'Build gem'
task :default => [:test]

task :compile => ['dealer:clean', 'dealer:compile']

task :test => :compile

task :build => :clean

Rake::TestTask.new do |t|
  t.libs << "lib" << 'spec/support'
  t.test_files = FileList['spec/**/*_spec.rb']
  t.verbose = false
  t.warning = false
end

def gemspec
  @clean_gemspec ||= eval(File.read(File.expand_path('../acpc_dealer.gemspec', __FILE__)))
end

Gem::PackageTask.new(gemspec) { |pkg| }
Rake::ExtensionTask.new('hand_evaluator', gemspec)

task :clean => 'dealer:clean' do
  sh "rm -f lib/hand_evaluator.so"
end

namespace :dealer do
  desc 'Compile ACPC dealer'
  task :compile do
    Dir.chdir(File.expand_path('../vendor/project_acpc_server', __FILE__)) do
      sh "make"
    end
  end

  desc 'Clean ACPC dealer'
  task :clean do
    Dir.chdir(File.expand_path('../vendor/project_acpc_server', __FILE__)) do
      sh "make clean"
    end
  end
end