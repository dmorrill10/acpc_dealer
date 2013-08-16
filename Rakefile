require "bundler/gem_tasks"
require 'rake'
require 'rake/extensiontask'
require 'rake/testtask'

require File.expand_path('../lib/acpc_dealer/version', __FILE__)

desc 'Build gem'
task :default => [:build, :test]

desc 'Compile ACPC dealer'
task :dealer do
  Dir.chdir(File.expand_path('../vendor/project_acpc_server', __FILE__)) do
    sh "make"
  end
end

task :build => [:clean, :compile, :dealer]

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

task :clean do
  sh "rm -f lib/hand_evaluator.so"

  Dir.chdir(File.expand_path('../vendor/project_acpc_server', __FILE__)) do
    sh "make clean"
  end
end

