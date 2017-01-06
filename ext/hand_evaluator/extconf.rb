
#Run 'ruby -r mkmf extconf.rb' to generate a Makefile

require 'mkmf'
root_dir = File.expand_path('../../../', __FILE__)
Dir.chdir(root_dir) do
  command = "mortar --store #{File.join(root_dir, '.bricks')}"
  puts "Getting Bricks (non-gem dependencies): `#{command}`"
  system command

  command = 'rake compile:dealer'
  puts "Compiling project_acpc_server: `#{command}`"
  system command
end
create_makefile("hand_evaluator")
