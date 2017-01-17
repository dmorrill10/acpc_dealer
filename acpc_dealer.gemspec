# -*- encoding: utf-8 -*-
require File.expand_path('../lib/acpc_dealer/version', __FILE__)

Gem::Specification.new do |gem|
  gem.name          = "acpc_dealer"
  gem.version       = AcpcDealer::VERSION
  gem.authors       = ["Dustin Morrill"]
  gem.email         = ["morrill@ualberta.ca"]
  gem.description   = %q{Annual Computer Poker Competition Dealer}
  gem.summary       = %q{Ruby interface to the Annual Computer Poker Competition's dealer program.}
  gem.homepage      = "https://github.com/dmorrill10/acpc_dealer"
  gem.license       = 'MIT'
  gem.platform      = Gem::Platform::RUBY

  gem.add_dependency 'clive', '~> 1.2'
  gem.add_dependency 'rake-compiler', '~> 0.8'
  gem.add_dependency 'brick_and_mortar', '~> 0.1'

  gem.add_development_dependency 'minitest', '~> 5.0'
  gem.add_development_dependency 'mocha', '~> 0.13'
  gem.add_development_dependency 'awesome_print', '~> 1.0'
  gem.add_development_dependency 'simplecov', '~> 0.7'

  gem.files         = Dir.glob("lib/**/*") + Dir.glob("ext/**/*") + Dir.glob('*.gemspec') +
    %w(Rakefile README.md Brickfile) + Dir.glob('spec/**/*')
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.extensions    = Dir.glob("ext/**/extconf.rb")
  gem.require_paths = ["lib"]
  gem.executables   << 'acpc_dealer'
end
