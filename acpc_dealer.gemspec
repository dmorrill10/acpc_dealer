# -*- encoding: utf-8 -*-
require File.expand_path('../lib/acpc_dealer/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["Dustin Morrill"]
  gem.email         = ["morrill@ualberta.ca"]
  gem.description   = %q{Annual Computer Poker Competition Dealer}
  gem.summary       = %q{Ruby interface to the Annual Computer Poker Competition's dealer program.}
  gem.homepage      = ""

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "acpc_dealer"
  gem.require_paths = ["lib"]
  gem.version       = AcpcDealer::VERSION

  gem.add_dependency 'rake-compiler'
  gem.add_development_dependency 'rspec'
  gem.add_development_dependency 'mocha'
  gem.add_development_dependency 'piston'
end
