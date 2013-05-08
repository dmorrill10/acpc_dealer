require_relative 'support/spec_helper'

require 'acpc_dealer'

include AcpcDealer

describe ConnectionInformation do
  it 'works' do
    port = 9001
    host = 'my host'
    millisecond_response_timeout = 100
    patient = ConnectionInformation.new(port, host, millisecond_response_timeout)

    patient.port_number.must_equal port
    patient.host_name.must_equal host
    patient.millisecond_response_timeout.must_equal millisecond_response_timeout
  end
end