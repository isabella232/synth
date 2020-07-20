# BOM

MCU: [STM32F407VGT6](https://www.mouser.com/ProductDetail/STMicroelectronics/STM32F407VGT6) LQFP-100

Crystal: [ATS08A](https://www.mouser.com/ProductDetail/CTS-Electronic-Components/ATS08A?qs=Mr%252bgrRYddfqUVH22tsgQmw%3D%3D)

Why: Recommended by [AN2867 Revision
3](http://easyelectronics.ru/img/ARM_kurs/STMstuff/stm32oscrecomend.pdf),
although the newest (Rev 11) doesn't mention it.


<<<<<<< HEAD
DAC options:
1. [CS4344](https://www.cirrus.com/products/cs4344-45-48/) - Easy to solder, cheap, available at JLCPCB's assembly if I choose to use that.
1. [TLV320DAC32](https://www.mouser.com/ProductDetail/Texas-Instruments/TLV320DAC32IRHBR?qs=sGAEpiMZZMtuedWc5vOqGAv3iOMzI%252BMxln3lJ9qKgHQ%3D)
1. [CS43L22](https://www.mouser.com/ProductDetail/Cirrus-Logic/CS43L22-CNZ) - What's on the STM32F4 discovery board.
1. [TLV320DAC26](https://www.mouser.com/ProductDetail/Texas-Instruments/TLV320DAC26IRHBR?qs=sGAEpiMZZMv%252bLKsN4bqzyiJsZIbtISVFY0fihiLYnzk%3d)

Headphone Jack: [CUI SJ1-3533NG](https://www.mouser.com/ProductDetail/CUI/SJ1-3533NG?qs=WyjlAZoYn53GjNky%252bNFGCA==)

Why: First one I found that didn't also have tip or ring switches in it.  Does
take up horizontal space but it can probably be spared.


Potentiometers:
-  [Bourne PTV09A](https://www.mouser.com/ProductDetail/Bourns/PTV09A-4025F-B103?qs=sGAEpiMZZMtC25l1F4XBU7WMi1wGK3ZvZ15ABC5GIFs%3d)
    Why: KiCad already had a footprint for these.
-  [ALPS RK09K1130A70](https://www.mouser.com/ProductDetail/ALPS/RK09K1130A70?qs=6EGMNY9ZYDSOs3JO8Oeg1w%3D%3D)
    Why: Cheaper than the above.


Buttons: [Mountain Switch Narrow](https://www.mouser.com/ProductDetail/Mountain-Switch/101-TS4311T1601-EV?qs=sGAEpiMZZMsgGjVA3toVBHVb%2f41oZGWlfjE497o3b4A%3d)

Why: Narrow width should help cram them all in there.  160g force should be easy to press.


Voltage Regulator: [LD1117 (3.3V)](https://www.mouser.com/ProductDetail/STMicroelectronics/LD1117S33TR)


Barrel Jack: [Adafruit 5.5/2.1mm](https://www.adafruit.com/product/373)
