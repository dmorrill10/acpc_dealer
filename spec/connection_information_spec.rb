require_relative 'support/spec_helper'

require 'acpc_dealer'

include AcpcDealer

describe ConnectionInformation do
  it 'works' do
    port = 9001
    host = 'my host'
    patient = ConnectionInformation.new(port, host)

    patient.port_number.must_equal port
    patient.host_name.must_equal host
  end
end