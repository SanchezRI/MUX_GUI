# Devices
-----------------------------------------------------------------------
<center>This project only for manipulating and monitoring JINR-devices for stochastic cooling system.</center>

<span style="color:green">NICA (Nuclotron-based Ion Collider fAсility) is a new accelerator complex designed at the Joint Institute for Nuclear Research (Dubna, Russia) to study properties of dense baryonic matter.</span>

## Device List

|Device Name|Description|Status|
|-----------|-----------|------|
|**_ADC/DAC Module_**|Changing beam's position in the NICA accelerator's communications |Development   |
|**_Commutator_**    |Summation of signal's amplitude on each stochastic cooling line|Beta-testing  |

## Commutator device
This is 3-unit device which communicate with client via **ModbusTCP** protocol.
It has 4 types of multiplexers with diffrent **MUX_ID**:
- _ID0_: Unknown type of slot or slot is empty
- _ID1_: Slot 1x(2 &rarr; 6)
- _ID2_: Slot 2x(4 &rarr; 1)
- _ID3_: Slot 1x(8 &rarr; 1)
- _ID4_: Slot 4x(2 &rarr; 1)

<span style="color:yellow">For identification of the slot type is responsible input register with 0x0002 address. </span>

<span style="color:yellow">Commutator controled with 3 holding registers with addresses 0x000A (Right slot), 0x000B (Middle slot), 0x000C (Left slot) </span>
