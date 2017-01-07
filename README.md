ACPC Dealer
============

[![Build Status](https://travis-ci.org/dmorrill10/acpc_dealer.svg?branch=master)](https://travis-ci.org/dmorrill10/acpc_dealer)

The [Annual Computer Poker Competition Dealer][ACPC Dealer GitHub] gem wraps the [<em>ACPC Dealer Server</em>][ACPC competition server] in a handy gem with a convenient runner class, and a script for compiling and running the dealer and example players.

## Installation

Add this line to your application's Gemfile:

    gem 'acpc_dealer'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install acpc_dealer


## Usage

Run

    $ acpc_dealer -h
to see a list of commands available in the command line interface.

See the [documentation][docs] for information on available classes, modules, and constants.

## Logging

For every match, two files will be logged: `<match name>.log` containing the match results and `<match name>.actions.log` containing the sequence of messages to and from the dealer over the course of the match. The location of these files can be customized with the `:log_directory`option. By default, they will be populated in a `<match name>.logs` subdirectory of the current working directory.

## Contributing

See the [issue tracker](https://github.com/dmorrill10/acpc_dealer/issues) for currently known issues, or to log new ones.

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Added some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

Please include tests with your changes, otherwise I may not be able to find 
the time to incorporate them into this project.

Copyright
---------
Copyright &copy; 2012-2017 by the Computer Poker Research Group, University of Alberta. See [LICENSE](LICENSE.md) for details.


<!---
    Link references
    ================
-->

[ACPC Dealer GitHub]: https://github.com/dmorrill10/acpc_dealer#readme
[ACPC competition server]: http://www.computerpokercompetition.org/index.php?option=com_rokdownloads&view=folder&Itemid=59
[docs]: http://rubydoc.info/github/dmorrill10/acpc_dealer/frames
