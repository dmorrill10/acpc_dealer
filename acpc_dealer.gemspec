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

  gem.add_dependency 'clive'
  gem.add_dependency 'dmorrill10-utils', '>= 1.0.0'
  gem.add_dependency 'rake-compiler'

  gem.add_development_dependency 'mocha'
  gem.add_development_dependency 'simplecov'
  gem.add_development_dependency 'turn'
  gem.add_development_dependency 'minitest'
  gem.add_development_dependency 'pry-rescue'

  gem.files         = Dir.glob("lib/**/*") + Dir.glob("ext/**/*") + Dir.glob('*.gemspec') +
    %w(Rakefile README.md) + Dir.glob('spec/**/*') + Dir.glob("vendor/**/*")
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.extensions    = Dir.glob("ext/**/extconf.rb")
  gem.require_paths = ["lib"]
  gem.executables   << 'acpc_dealer'
end
