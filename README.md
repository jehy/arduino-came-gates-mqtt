# Arduino came gates mqtt

[![Compile Sketch](https://github.com/jehy/arduino-came-gates-mqtt/actions/workflows/compile-sketch.yaml/badge.svg)](https://github.com/jehy/arduino-came-reader/actions/workflows/compile-sketch.yaml)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.me/jehyrus)

Open CAME gates using Arduino and 433 Mhz transmitter, based on [this sketch](https://gist.github.com/superyarik/3eb4da9da728466c072e716532d732ef).

You can obtain your gate's code using [this sketch](https://github.com/jehy/arduino-came-reader).

Works by MQTT. Just send any payload to configured topic and it will open gates.

Sample config for Home Assistant:

```yaml
switch:
  - platform: mqtt
    unique_id: gates_btn
    name: "Gates Switch"
    command_topic: "esp/street/gates/open"
    availability:
      - topic: "esp/street/gates/available"
    qos: 0
    retain: false
```

# ESPHome
ESPHome version can be found in `esphome` directory.

Just copy `esphome/settings.sample.h` to `esphome/settings.h`, tweak it and you are ready for upload :) 
