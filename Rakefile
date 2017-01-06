require "bundler/gem_tasks"
require 'rake'
require 'rake/clean'
require 'rbconfig'
require 'rake/testtask'
require 'rake/extensiontask'
require 'rubygems/package_task'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

task :default => :test

desc 'Compile hand_evaluator and dealer'
task :compile => ['compile:hand_evaluator', 'compile:dealer']

desc 'Run tests'
task :test => :compile

task :build => :clean

desc 'Remove compiled products'
task :clean => ['clean:hand_evaluator', 'clean:dealer']

Rake::TestTask.new do |t|
  t.libs << "lib" << 'spec/support'
  t.test_files = FileList['spec/**/*_spec.rb']
  t.verbose = false
  t.warning = false
end

def gemspec
  @clean_gemspec ||= eval(File.read(File.expand_path('../acpc_dealer.gemspec', __FILE__)))
end

Gem::PackageTask.new(gemspec)
Rake::ExtensionTask.new('hand_evaluator', gemspec)

namespace :compile do
  desc 'Compile ACPC dealer'
  task :dealer do
    Dir.chdir(File.expand_path('../vendor/project_acpc_server', __FILE__)) do
      sh "make"
    end
  end
end

namespace :clean do
  desc 'Clean hand_evaluator'
  task :hand_evaluator do
    sh "rm -f #{File.expand_path('../lib/hand_evaluator.so')}"
  end

  desc 'Clean ACPC dealer'
  task :dealer do
    Dir.chdir(File.expand_path('../vendor/project_acpc_server', __FILE__)) do
      sh "make clean"
    end
  end
end
