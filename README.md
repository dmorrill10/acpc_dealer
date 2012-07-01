# AcpcDealer

TODO: Write a gem description

## Installation

Add this line to your application's Gemfile:

    gem 'acpc_dealer'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install acpc_dealer

## Usage

TODO: Write usage instructions here

## Logging

For every match, two files will be logged: `<match name>.log` containing the match results and `<match name>.actions.log` containing the sequence of messages to and from the dealer over the course of the match. The location of these files can be customized with the :log_directory option. By default, they will be populated in the working directory of the calling process.

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Added some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
