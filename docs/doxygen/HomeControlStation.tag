<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.18.0">
  <compound kind="file">
    <name>DoxygenMainPage.md</name>
    <path>docs/</path>
    <filename>DoxygenMainPage_8md.html</filename>
  </compound>
  <compound kind="file">
    <name>AdvancedControls.hpp</name>
    <path>include/devices/</path>
    <filename>AdvancedControls_8hpp.html</filename>
    <class kind="struct">AdvancedControlsOnOff</class>
    <class kind="struct">LedColor</class>
    <class kind="struct">LedStripAnimationProperties</class>
    <member kind="enumeration">
      <type></type>
      <name>Direction</name>
      <anchorfile>AdvancedControls_8hpp.html</anchorfile>
      <anchor>a224b9163917ac32fc95a60d8c1eec3aa</anchor>
      <arglist></arglist>
      <enumvalue file="AdvancedControls_8hpp.html" anchor="a224b9163917ac32fc95a60d8c1eec3aaad1e80c506f4c89e3c4fb1c1b4867a03c">LeftToRight</enumvalue>
      <enumvalue file="AdvancedControls_8hpp.html" anchor="a224b9163917ac32fc95a60d8c1eec3aaab7b0ea3028791689ea070674776855e2">RightToLeft</enumvalue>
    </member>
  </compound>
  <compound kind="file">
    <name>HwButton.hpp</name>
    <path>include/devices/CustomHardware/</path>
    <filename>HwButton_8hpp.html</filename>
    <class kind="class">HwButton</class>
  </compound>
  <compound kind="file">
    <name>device.hpp</name>
    <path>include/devices/</path>
    <filename>device_8hpp.html</filename>
    <includes id="NvmConfigSlotDefinition_8hpp" name="NvmConfigSlotDefinition.hpp" local="yes" import="no" module="no" objc="no">os/datacontainer/NvmConfigSlotDefinition.hpp</includes>
    <includes id="AdvancedControls_8hpp" name="AdvancedControls.hpp" local="yes" import="no" module="no" objc="no">AdvancedControls.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="class">Device</class>
    <class kind="struct">DeviceDescription</class>
    <class kind="struct">RtcTime</class>
    <class kind="struct">ServiceParameters_set1</class>
    <class kind="struct">ServiceParameters_set2</class>
    <class kind="struct">ServiceParameters_set3</class>
    <member kind="define">
      <type>#define</type>
      <name>DEVICE_NAME_MAX_LENGHT</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a81bd54af79993b2990e5bd293c79468f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a603b5c1c689a321dbc1bf0a69538fd38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SERVICE_NAME_INDEX</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a3852d640ad699285856f473876bf530a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SERVICE_OVERLOADING_FUNCTION_INDEX</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>abf8d36416fef4a6a5bb683c6ce0d319d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>DeviceServicesType</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_ENABLE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3abfe891e52cbdb9fee7a548e719c9c9e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_DISABLE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3ab50a4bac1a2f3adb638b3f5f1f40fb61</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_SET_DATA</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a12ad0e5cb3f5adb7c5484bf68b3ea45c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_GET_DATA</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a2a3d40625eb57260a523b8d12cf47ee0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_STATE_SWITCH</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a8737a1ecada1e2faa4802de6f4f18d7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_LED_STRIP_SAVE_CONTENT</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a47ec97f139bc3668a9b79b9dea3d6d47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_LED_STRIP_SWITCH_CONTENT</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a5c914dec6f1ceb29e7edfa554fe13227</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_BRIGHTNESS_CHANGE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a1811c7809299dac9d8ec82a02c886db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_SET_EXT_MEMORY_PTR</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a04e3923befa23f222a75ecb7ed3dd029</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_GET_ADVANCED_CONTROLS</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3aa0ba4767de01df07799d7c9c3fe326ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_GET_DETAILED_COLORS</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3ab9d299abab11bc447146cfae2aa49fdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_SET_DETAILED_COLORS</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a517e4be500cf3ffd7e761ac0cc9a34be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_ROOM_STATE_CHANGE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3ae8d86b71a38b72dcc973c0f2406e7f7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_SEGMENT_STATE_SWITCH</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a8c18d5e91eb8a193017069ced5684097</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_LIVE_ANIMATION</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a2b046e5f4ac31337ebf86a46119a92b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVSERVICE_INVALID</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a85b136736229097bd3d73d2e51d9e0f3a5d6207d4ed5567d0d0ebc83b11c0424a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>DevType</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_ONOFFDEVICE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46ba4681d42feaecce95f3969e3adbd148e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_LED_STRIP</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46badc0f1f44d57864572222ed5f4ecf2054</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_TEMP_SENSOR</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46bae877adb07bf73f323b1ab3d522281262</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_LED_STRIP_SEGMENTED</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46ba592bb49e84bfcedce4210f6575f033af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_DISTANCE_SENSOR</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46ba96174c5862b93157edd19f36be5790f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_HARDWARE_BUTTON</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46baeed38a5239ba6c989571d8f1e19c779a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>type_DEVICE_TYPE_LAST</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>acc6818b8521baa7c7802b20cd44ff46bad5b98bac129b434ef127e0a5f2840017</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ServiceDirectionType</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a64d03546e9e332a5232ec04c1261fdcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_OUT_from_DEVICE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a64d03546e9e332a5232ec04c1261fdcdadca70b624958554d99940d60b77e2dc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_IN_to_DEVICE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a64d03546e9e332a5232ec04c1261fdcda6d6301edcc5a3a419b7df479088fa980</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_UNKNOWN_PARAM_DIRECTION</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a64d03546e9e332a5232ec04c1261fdcdaf2804d0b6e82a2b42628cba70dc8a97a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ServiceOverloadingFunction</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a89ed5b2e43e0258470ec042402d4bfe9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>serviceCall_NoParams</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a89ed5b2e43e0258470ec042402d4bfe9ae097ec21390cfca4d05fef0ef4031790</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>serviceCall_1</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a89ed5b2e43e0258470ec042402d4bfe9a31b818f85efa2bbc593c9b99a81bb997</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>serviceCall_2</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a89ed5b2e43e0258470ec042402d4bfe9a2adc731f1aee47e52bbd001d22b0419d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>serviceCall_3</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a89ed5b2e43e0258470ec042402d4bfe9a03b0e1aadddbb050e7c803668f425183</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>serviceCall_INVALID</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>a89ed5b2e43e0258470ec042402d4bfe9a0b9ee89002adf37eacf6b1db4938c735</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ServiceRequestErrorCode</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERV_SUCCESS</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931eba099b823edaee5c5131cbe6a4cb92c466</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERV_EXECUTION_FAILURE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931ebae44eaed181dfc07a7a9b7baba5802878</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERV_GENERAL_FAILURE</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931eba26c2b260f46dbc23eda93f8761b96a52</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERV_BUSY</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931eba20552cfac73f7e78e86c0510ef439f48</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERV_PENDING</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931ebafc74ef2e2a50dcbfc10155f78675dcd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERV_NOT_SUPPORTED</name>
      <anchorfile>device_8hpp.html</anchorfile>
      <anchor>ad905eb84f911940c72b89af14f6931eba418102ffa28ee4498922f4a03b06db14</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>distanceSensor.hpp</name>
    <path>include/devices/</path>
    <filename>distanceSensor_8hpp.html</filename>
    <class kind="class">DistanceSensor</class>
  </compound>
  <compound kind="file">
    <name>BounceInAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>BounceInAnimation_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">BounceInAnimation</class>
  </compound>
  <compound kind="file">
    <name>FadeIn.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>FadeIn_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">FadeInAnimation</class>
  </compound>
  <compound kind="file">
    <name>FadeOut.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>FadeOut_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">FadeOutAnimation</class>
  </compound>
  <compound kind="file">
    <name>ILedAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>ILedAnimation_8hpp.html</filename>
    <includes id="AdvancedControls_8hpp" name="AdvancedControls.hpp" local="yes" import="no" module="no" objc="no">devices/AdvancedControls.hpp</includes>
    <class kind="class">ILedAnimation</class>
  </compound>
  <compound kind="file">
    <name>ILiveAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>ILiveAnimation_8hpp.html</filename>
  </compound>
  <compound kind="file">
    <name>SingleWaveAnimation copy.hpp</name>
    <path>include/devices/LedStrip/Animations/live/</path>
    <filename>SingleWaveAnimation_01copy_8hpp.html</filename>
  </compound>
  <compound kind="file">
    <name>SingleWaveAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/live/</path>
    <filename>SingleWaveAnimation_8hpp.html</filename>
    <includes id="ILiveAnimation_8hpp" name="ILiveAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/ILiveAnimation.hpp</includes>
    <class kind="class">ComplexSequenceAnimation::Burst</class>
    <class kind="class">ComplexSequenceAnimation</class>
    <class kind="class">Config</class>
    <class kind="class">ILiveAnimation</class>
    <class kind="class">SingleWaveAnimation</class>
    <class kind="class">SmoothWaveAnimation</class>
  </compound>
  <compound kind="file">
    <name>RollInAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>RollInAnimation_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">RollInAnimation</class>
  </compound>
  <compound kind="file">
    <name>RollOutAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>RollOutAnimation_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">RollOutAnimation</class>
  </compound>
  <compound kind="file">
    <name>SparkleInAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>SparkleInAnimation_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">SparkleInAnimation</class>
  </compound>
  <compound kind="file">
    <name>TwinkleInAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>TwinkleInAnimation_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">TwinkleInAnimation</class>
  </compound>
  <compound kind="file">
    <name>WaveInAnimation.hpp</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>WaveInAnimation_8hpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">ILedAnimation.hpp</includes>
    <class kind="class">WaveInAnimation</class>
  </compound>
  <compound kind="file">
    <name>LedWS1228b.hpp</name>
    <path>include/devices/LedStrip/</path>
    <filename>LedWS1228b_8hpp.html</filename>
    <includes id="SystemDefinition_8hpp" name="SystemDefinition.hpp" local="no" import="no" module="no" objc="no">SystemDefinition.hpp</includes>
    <includes id="FadeIn_8hpp" name="FadeIn.hpp" local="yes" import="no" module="no" objc="no">Animations/FadeIn.hpp</includes>
    <includes id="FadeOut_8hpp" name="FadeOut.hpp" local="yes" import="no" module="no" objc="no">Animations/FadeOut.hpp</includes>
    <includes id="RollInAnimation_8hpp" name="RollInAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/RollInAnimation.hpp</includes>
    <includes id="RollOutAnimation_8hpp" name="RollOutAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/RollOutAnimation.hpp</includes>
    <includes id="SparkleInAnimation_8hpp" name="SparkleInAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/SparkleInAnimation.hpp</includes>
    <includes id="WaveInAnimation_8hpp" name="WaveInAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/WaveInAnimation.hpp</includes>
    <includes id="TwinkleInAnimation_8hpp" name="TwinkleInAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/TwinkleInAnimation.hpp</includes>
    <includes id="BounceInAnimation_8hpp" name="BounceInAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/BounceInAnimation.hpp</includes>
    <includes id="SingleWaveAnimation_8hpp" name="SingleWaveAnimation.hpp" local="yes" import="no" module="no" objc="no">Animations/live/SingleWaveAnimation.hpp</includes>
    <class kind="class">LedWS1228bDeviceType</class>
    <member kind="define">
      <type>#define</type>
      <name>DEFAULT_TICKS_TO_ANIMATE</name>
      <anchorfile>LedWS1228b_8hpp.html</anchorfile>
      <anchor>a9d6f448b7661c7824335bf69d70c7920</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Segmented_LedWS1228b.hpp</name>
    <path>include/devices/LedStrip/</path>
    <filename>Segmented__LedWS1228b_8hpp.html</filename>
    <includes id="SystemDefinition_8hpp" name="SystemDefinition.hpp" local="no" import="no" module="no" objc="no">SystemDefinition.hpp</includes>
    <includes id="FadeIn_8hpp" name="FadeIn.hpp" local="yes" import="no" module="no" objc="no">Animations/FadeIn.hpp</includes>
    <includes id="FadeOut_8hpp" name="FadeOut.hpp" local="yes" import="no" module="no" objc="no">Animations/FadeOut.hpp</includes>
    <class kind="class">SegLedWS1228bDeviceType</class>
    <class kind="class">SegmentData</class>
  </compound>
  <compound kind="file">
    <name>onoffdevice.hpp</name>
    <path>include/devices/</path>
    <filename>onoffdevice_8hpp.html</filename>
    <class kind="class">OnOffDevice</class>
  </compound>
  <compound kind="file">
    <name>tempSensorDHT11.hpp</name>
    <path>include/devices/</path>
    <filename>tempSensorDHT11_8hpp.html</filename>
    <includes id="SystemDefinition_8hpp" name="SystemDefinition.hpp" local="no" import="no" module="no" objc="no">SystemDefinition.hpp</includes>
    <class kind="struct">TempSensorDHT11DeviceType::SensorReading</class>
    <class kind="class">TempSensorDHT11DeviceType</class>
  </compound>
  <compound kind="file">
    <name>TestDeviceType.hpp</name>
    <path>include/devices/</path>
    <filename>TestDeviceType_8hpp.html</filename>
    <class kind="class">TestDeviceType</class>
  </compound>
  <compound kind="file">
    <name>configprovider.hpp</name>
    <path>include/os/app/config/</path>
    <filename>configprovider_8hpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="struct">ConfigData</class>
    <class kind="class">ConfigProvider</class>
    <member kind="define">
      <type>#define</type>
      <name>PASSWORD_LENGTH</name>
      <anchorfile>configprovider_8hpp.html</anchorfile>
      <anchor>a0bbe35fda939018d5672a6cfc02cfa51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SSID_LENGTH</name>
      <anchorfile>configprovider_8hpp.html</anchorfile>
      <anchor>af8acf478fce4b1d4c9d1226ade8fd243</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ExtendedMemoryManager.hpp</name>
    <path>include/os/app/config/</path>
    <filename>ExtendedMemoryManager_8hpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="class">ExtendedMemoryManager</class>
    <class kind="struct">ExtMemoryData</class>
    <class kind="struct">ExtMemoryMetadataType</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_EXT_MEMORY_SIZE_TOTAL</name>
      <anchorfile>ExtendedMemoryManager_8hpp.html</anchorfile>
      <anchor>ab12f80aa5208f7220a57d5c2687994e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SLOTS_FOR_EXT_MEMORY_BUFFERS</name>
      <anchorfile>ExtendedMemoryManager_8hpp.html</anchorfile>
      <anchor>a055db83c5835a462f852ae413738bb87</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>persistentMemoryAccess.hpp</name>
    <path>include/os/app/config/</path>
    <filename>persistentMemoryAccess_8hpp.html</filename>
    <class kind="class">PersistentMemoryAccess</class>
  </compound>
  <compound kind="file">
    <name>devicemanager.hpp</name>
    <path>include/os/app/</path>
    <filename>devicemanager_8hpp.html</filename>
    <includes id="TestDeviceType_8hpp" name="TestDeviceType.hpp" local="no" import="no" module="no" objc="no">devices/TestDeviceType.hpp</includes>
    <includes id="LedWS1228b_8hpp" name="LedWS1228b.hpp" local="no" import="no" module="no" objc="no">devices/LedStrip/LedWS1228b.hpp</includes>
    <includes id="Segmented__LedWS1228b_8hpp" name="Segmented_LedWS1228b.hpp" local="no" import="no" module="no" objc="no">devices/LedStrip/Segmented_LedWS1228b.hpp</includes>
    <includes id="tempSensorDHT11_8hpp" name="tempSensorDHT11.hpp" local="no" import="no" module="no" objc="no">devices/tempSensorDHT11.hpp</includes>
    <includes id="distanceSensor_8hpp" name="distanceSensor.hpp" local="no" import="no" module="no" objc="no">devices/distanceSensor.hpp</includes>
    <includes id="HwButton_8hpp" name="HwButton.hpp" local="no" import="no" module="no" objc="no">devices/CustomHardware/HwButton.hpp</includes>
    <includes id="ExtendedDataAllocator_8hpp" name="ExtendedDataAllocator.hpp" local="no" import="no" module="no" objc="no">os/tools/ExtendedDataAllocator.hpp</includes>
    <class kind="class">DeviceManager</class>
  </compound>
  <compound kind="file">
    <name>deviceProvider.hpp</name>
    <path>include/os/app/</path>
    <filename>deviceProvider_8hpp.html</filename>
    <class kind="class">DeviceProvider</class>
    <class kind="struct">DeviceTranslationDetails</class>
  </compound>
  <compound kind="file">
    <name>DigitalEventDefinitions.hpp</name>
    <path>include/os/app/DigitalEvent/</path>
    <filename>DigitalEventDefinitions_8hpp.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DIGITAL_EVENT_CONFIRMED_MSG_ID</name>
      <anchorfile>DigitalEventDefinitions_8hpp.html</anchorfile>
      <anchor>a2d28fcacc9f8a979ba4cc21fc5d58611</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DIGITAL_EVENT_FIRED_MSG_ID</name>
      <anchorfile>DigitalEventDefinitions_8hpp.html</anchorfile>
      <anchor>a05b2eb9e46f3c3dd1cbf0ea089e97671</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DigitalEventReceiver.hpp</name>
    <path>include/os/app/DigitalEvent/</path>
    <filename>DigitalEventReceiver_8hpp.html</filename>
    <includes id="DigitalEventDefinitions_8hpp" name="DigitalEventDefinitions.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventDefinitions.hpp</includes>
    <class kind="class">DigitalEventReceiver</class>
    <class kind="struct">ServiceCallData</class>
  </compound>
  <compound kind="file">
    <name>DigitalEventTransmitter.hpp</name>
    <path>include/os/app/DigitalEvent/</path>
    <filename>DigitalEventTransmitter_8hpp.html</filename>
    <includes id="DigitalEventDefinitions_8hpp" name="DigitalEventDefinitions.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventDefinitions.hpp</includes>
    <class kind="class">DigitalEventTransmitter</class>
  </compound>
  <compound kind="file">
    <name>DeviceDescriptionPacker.hpp</name>
    <path>include/os/app/display/</path>
    <filename>DeviceDescriptionPacker_8hpp.html</filename>
    <includes id="device_8hpp" name="device.hpp" local="yes" import="no" module="no" objc="no">devices/device.hpp</includes>
    <class kind="class">DeviceDescriptionPacker</class>
  </compound>
  <compound kind="file">
    <name>Display.hpp</name>
    <path>include/os/app/display/</path>
    <filename>Display_8hpp.html</filename>
    <includes id="messageUDP_8hpp" name="messageUDP.hpp" local="yes" import="no" module="no" objc="no">os/tools/messageUDP.hpp</includes>
    <includes id="DisplayDatatypes_8hpp" name="DisplayDatatypes.hpp" local="yes" import="no" module="no" objc="no">os/app/display/DisplayDatatypes.hpp</includes>
    <class kind="class">Display</class>
  </compound>
  <compound kind="file">
    <name>DisplayDatatypes.hpp</name>
    <path>include/os/app/display/</path>
    <filename>DisplayDatatypes_8hpp.html</filename>
    <includes id="networkdriver_8hpp" name="networkdriver.hpp" local="yes" import="no" module="no" objc="no">os/drivers/networkdriver.hpp</includes>
    <class kind="struct">DisplayInfo</class>
    <member kind="enumeration">
      <type></type>
      <name>DisplayMessageIds</name>
      <anchorfile>DisplayDatatypes_8hpp.html</anchorfile>
      <anchor>ad5a125afd884475dcb5e76dca6b31091</anchor>
      <arglist></arglist>
      <enumvalue file="DisplayDatatypes_8hpp.html" anchor="ad5a125afd884475dcb5e76dca6b31091a6099c8000260cd8c46b95ae349c463ec">DISCOVER_DISPLAYS</enumvalue>
      <enumvalue file="DisplayDatatypes_8hpp.html" anchor="ad5a125afd884475dcb5e76dca6b31091ae9b6bcb4396ea332592d2d092c23decc">DISPLAY_PRESENT</enumvalue>
    </member>
  </compound>
  <compound kind="file">
    <name>DisplayServer.hpp</name>
    <path>include/os/app/display/</path>
    <filename>DisplayServer_8hpp.html</filename>
    <includes id="Display_8hpp" name="Display.hpp" local="yes" import="no" module="no" objc="no">os/app/display/Display.hpp</includes>
    <class kind="class">DisplayServer</class>
    <member kind="define">
      <type>#define</type>
      <name>DISPLAY_DISCOVERY_FREQ_MS</name>
      <anchorfile>DisplayServer_8hpp.html</anchorfile>
      <anchor>a0149a240f7416c55de461f3dc9bd8924</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DISPLAY_HASH_REFRESH_FREQ_MS</name>
      <anchorfile>DisplayServer_8hpp.html</anchorfile>
      <anchor>a2bf97f4db425640040770dcb45d6fb38</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AdvancedControlsLoader.hpp</name>
    <path>include/os/app/http/</path>
    <filename>AdvancedControlsLoader_8hpp.html</filename>
    <includes id="device_8hpp" name="device.hpp" local="no" import="no" module="no" objc="no">devices/device.hpp</includes>
    <class kind="class">AdvancedControlsLoader</class>
  </compound>
  <compound kind="file">
    <name>configPageHttp.h</name>
    <path>include/os/app/http/</path>
    <filename>configPageHttp_8h.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>popupContent</name>
      <anchorfile>configPageHttp_8h.html</anchorfile>
      <anchor>a1d8d2426c4a9ee8943cd43d3bb56de9c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>deviceWidgets.h</name>
    <path>include/os/app/http/</path>
    <filename>deviceWidgets_8h.html</filename>
    <member kind="variable">
      <type>const String</type>
      <name>deviceWidgetsJS</name>
      <anchorfile>deviceWidgets_8h.html</anchorfile>
      <anchor>a516c8722c7acc0728c7434dfde381f80</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HttpAsyncRequestHandler.hpp</name>
    <path>include/os/app/http/</path>
    <filename>HttpAsyncRequestHandler_8hpp.html</filename>
    <includes id="device_8hpp" name="device.hpp" local="no" import="no" module="no" objc="no">devices/device.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="struct">HTTPAsyncRequestHandler::AsyncHttpRequest</class>
    <class kind="class">HTTPAsyncRequestHandler</class>
    <member kind="define">
      <type>#define</type>
      <name>DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ac04f6f61708711837a1b6031d4a90a35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DYNAMIC_REQUEST_ADDITIONAL_PARAM_IDX</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ab7d0e4838a8bdf31deb79453374dba08</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DYNAMIC_REQUEST_DIRECTION_IDX</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>afd5708f941ab9d8c8497856239d6b79e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DYNAMIC_REQUEST_MEMORY_LENGTH_IDX</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>abb53b3ed02f54e1a90a554a2d9b4431c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DYNAMIC_REQUEST_START_OF_DATA_IDX</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ae4f647f30997935aa744dbe7d7994422</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad2990263f6cdc6e762943e30ea459797</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>AsyncRequestState</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>acd59cb757fb13f9dfaa6b753c2c65f64</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_NO_REQUEST</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>acd59cb757fb13f9dfaa6b753c2c65f64a56c79af0c3eaa7f97be0ffc8db5f758f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_REQUEST_RECEIVED</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>acd59cb757fb13f9dfaa6b753c2c65f64a612a62f0e0852c826d4dd006d391038e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_REQUEST_PROCESSING</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>acd59cb757fb13f9dfaa6b753c2c65f64a8bb33a53ac1fdb2a9222066365d20a01</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_REQUEST_COMPLETED</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>acd59cb757fb13f9dfaa6b753c2c65f64a452496cd36f2c8d365b3f3f1dbbb4f57</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>AsyncRequestType</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_TYPE_INVALID</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a035f82f61e0aa64389715eec900e13c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_TYPE_DEVICE_SERVICE_CALL</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a0fea84258f8f7e1e9d9c9567330469cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_GET_PAGE_CONTENT</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892aeff04049a1d71cfd3dc54513a2257580</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_GET_HASH</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a7c0a32cefeb27848efc60fcefb1de886</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_GET_SYSTEM_DETAILS</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a4c23fe19a4cab5e9039d433013545324</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_GET_NOTIFICATION_LIST</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a96a32f8ef4134ee340d7f23d5eec9908</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_DOWNLOAD_CONFIGURATION</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a01a4aace4fc2230b20d9d9180753c883</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_REDIRECT_TO_MAIN_PAGE</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a4f95fd3113ca9835d0145e88d63bf408</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_GET_ADVANCED_CONTROLS</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892ac8d7aec1cddaee2751fe46e81f302505</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ASYNC_TYPE_LAST</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>ad895f32645faa6ed34deee86bcd3b892a08eff2ed9a2811ada53e7fdf9e3973fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>HttpErrorCode</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>a5be90f01766c5b0992aed84e923364c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_HTTP_OK</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>a5be90f01766c5b0992aed84e923364c8aba2dce73722872df4b2a5adcde8ace6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_HTTP_FAILED</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>a5be90f01766c5b0992aed84e923364c8a9f882d661c1191bf1d115737406f7a73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_HTTP_BUSY</name>
      <anchorfile>HttpAsyncRequestHandler_8hpp.html</anchorfile>
      <anchor>a5be90f01766c5b0992aed84e923364c8ac537a2a0dfb6e3d3285f54faf73dbf45</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>httpserver.hpp</name>
    <path>include/os/app/http/</path>
    <filename>httpserver_8hpp.html</filename>
    <includes id="HttpAsyncRequestHandler_8hpp" name="HttpAsyncRequestHandler.hpp" local="no" import="no" module="no" objc="no">os/app/http/HttpAsyncRequestHandler.hpp</includes>
    <class kind="class">HomeLightHttpServer</class>
    <class kind="struct">HomeLightHttpServer::HttpServerNvmMetadata</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUMBER_OF_ROOM_NAME_TRANSLATIONS</name>
      <anchorfile>httpserver_8hpp.html</anchorfile>
      <anchor>aca881f79574680062a90710b4e497573</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>javascript.h</name>
    <path>include/os/app/http/</path>
    <filename>javascript_8h.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>javascript</name>
      <anchorfile>javascript_8h.html</anchorfile>
      <anchor>a41be79f6041e28c778074551f625175d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pageHead.h</name>
    <path>include/os/app/http/</path>
    <filename>pageHead_8h.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>pageHead</name>
      <anchorfile>pageHead_8h.html</anchorfile>
      <anchor>a64d04cd305b7e44935fe73aebb8ff692</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>renderRoomsJS.h</name>
    <path>include/os/app/http/</path>
    <filename>renderRoomsJS_8h.html</filename>
    <includes id="deviceWidgets_8h" name="deviceWidgets.h" local="yes" import="no" module="no" objc="no">deviceWidgets.h</includes>
    <member kind="variable">
      <type>const String</type>
      <name>renderRoomsJS</name>
      <anchorfile>renderRoomsJS_8h.html</anchorfile>
      <anchor>ae3247219df689acf07848b719df37c28</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>style.h</name>
    <path>include/os/app/http/</path>
    <filename>style_8h.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>style_css</name>
      <anchorfile>style_8h.html</anchorfile>
      <anchor>a392c344f13c9adf751ce2768775204b8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tempGaugeCSS.h</name>
    <path>include/os/app/http/tempGauge/</path>
    <filename>tempGaugeCSS_8h.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>tempGaugeCSS</name>
      <anchorfile>tempGaugeCSS_8h.html</anchorfile>
      <anchor>af2a3bdb2783ced686fd8a06be1bece88</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tempGaugeJS.h</name>
    <path>include/os/app/http/tempGauge/</path>
    <filename>tempGaugeJS_8h.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>tempGaugeJS</name>
      <anchorfile>tempGaugeJS_8h.html</anchorfile>
      <anchor>a9b37512f081671abc2a4bc5bc1fe6c0b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>NotificationHandler.hpp</name>
    <path>include/os/app/</path>
    <filename>NotificationHandler_8hpp.html</filename>
    <class kind="class">NotificationHandler</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUMBER_OF_NOTIFICATIONS</name>
      <anchorfile>NotificationHandler_8hpp.html</anchorfile>
      <anchor>acd56b9fba69bcdc9e561bb4a4c304b52</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>rc_dataTypes.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>rc__dataTypes_8hpp.html</filename>
    <includes id="device_8hpp" name="device.hpp" local="no" import="no" module="no" objc="no">devices/device.hpp</includes>
    <class kind="struct">KeepAliveData</class>
    <class kind="struct">NodeInitialData</class>
    <member kind="enumeration">
      <type></type>
      <name>UdpFrames_RCS</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REQUEST_NODE_INITIAL_DATA</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69afc48d6e3c5bd12bc285fab81e51e8cc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REQUEST_NODE_DETAILED_DATA</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69a71107bc1e01bf9fed9acd7554acccb7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REQUEST_KEEP_ALIVE</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69a75bcec21a990f48ec5e589d9e9921b06</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_NODE_INITIAL_DATA</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69af1f680dc6ea9db891139c9368122acab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_NODE_DETAILED_DATA</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69a17514db6c293e247bdf569c917056c5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_KEEP_ALIVE</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69ae578b11d6e9bb2bf8b4152332282f842</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_REQUEST</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69ac4c8a64af1ec05ced9f0927a437109a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_RESPONSE</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69ad9b23d81200497aa5709860d19946749</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69a3488ce6815352c326e3bfbfb3b3ffe4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69ab078cbf52256b6f800f5e0aa8567575a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DISCOVER_ME_MESSAGE</name>
      <anchorfile>rc__dataTypes_8hpp.html</anchorfile>
      <anchor>a39cbc60c3e000bd265826192a95cca69a114b1e2fd70ab8bda8686f7c8e2eb0c5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RC_publicDataTypes.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>RC__publicDataTypes_8hpp.html</filename>
    <includes id="device_8hpp" name="device.hpp" local="no" import="no" module="no" objc="no">devices/device.hpp</includes>
    <includes id="rcRequest_8hpp" name="rcRequest.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/rcRequest.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <includes id="rcResponse_8hpp" name="rcResponse.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/rcResponse.hpp</includes>
    <class kind="struct">RcResponseLong</class>
    <member kind="define">
      <type>#define</type>
      <name>REQEST_SIZE</name>
      <anchorfile>RC__publicDataTypes_8hpp.html</anchorfile>
      <anchor>a0b3f023f2430bbdfc8257be0f6498977</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REQUEST_DATA_SIZE</name>
      <anchorfile>RC__publicDataTypes_8hpp.html</anchorfile>
      <anchor>a84f54e2d4430de0f1216f2b0aefc5f2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RESPONSE_DATA_SIZE</name>
      <anchorfile>RC__publicDataTypes_8hpp.html</anchorfile>
      <anchor>ade2c020b5e01bd748fa9d1610f824108</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RESPONSE_DATA_SIZE_LONG</name>
      <anchorfile>RC__publicDataTypes_8hpp.html</anchorfile>
      <anchor>a46ba8a919647f8d4375e9bc75d9b67d3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>rcRequest.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>rcRequest_8hpp.html</filename>
    <class kind="class">RcRequest</class>
    <member kind="define">
      <type>#define</type>
      <name>RC_REQUEST_MIN_SIZE</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>a8ebd3d62313dadbe812883d4db4a0938</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>RequestType</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>ae10b07f2d0feb103db7fe4cfd192e5af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REQ_FIRST</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>ae10b07f2d0feb103db7fe4cfd192e5afa70d3bbc8df8d1f8b0593edd1098e0894</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVICE_CALL_REQ</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>ae10b07f2d0feb103db7fe4cfd192e5afaef72f26519664a2c778b66fe7c22e318</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EXTENDED_DATA_DOWNLOAD_REQ</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>ae10b07f2d0feb103db7fe4cfd192e5afa5c3ba3bee49cd2da5394f77ffde3d19e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNKNOWN_REQ</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>ae10b07f2d0feb103db7fe4cfd192e5afa280fd0988d2e70bd4fc2cadae012491f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REQ_COUNT</name>
      <anchorfile>rcRequest_8hpp.html</anchorfile>
      <anchor>ae10b07f2d0feb103db7fe4cfd192e5afa6a01c85da250fba862d6c29d2d0168c4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>rcResponse.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>rcResponse_8hpp.html</filename>
    <includes id="rcRequest_8hpp" name="rcRequest.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/rcRequest.hpp</includes>
    <class kind="class">RcResponse</class>
    <member kind="define">
      <type>#define</type>
      <name>RC_RESPONSE_MIN_SIZE</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>a01f004c2c0cf56e122c74f7aa183d911</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ResponseType</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>af5055605e320b3f9ea9fa330b57604e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>POSITIVE_RESP</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>af5055605e320b3f9ea9fa330b57604e7a829456d0cc42186d45a836a3895eaf4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NEGATIVE_RESP</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>af5055605e320b3f9ea9fa330b57604e7af38a8ba1cc885ec57ea5ebba1363159d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_REQ_RESP</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>af5055605e320b3f9ea9fa330b57604e7a001e1474aa45005a2f8b767d41a95ee5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EXTENDED_DATA_DOWNLOAD_RESP</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>af5055605e320b3f9ea9fa330b57604e7a6927beb8ab7de8f990d7a82e19510003</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNKNOWN_RESP</name>
      <anchorfile>rcResponse_8hpp.html</anchorfile>
      <anchor>af5055605e320b3f9ea9fa330b57604e7a00c22670cde290c73c1bc704841fd4be</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>remoteControlClient.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>remoteControlClient_8hpp.html</filename>
    <class kind="class">RemoteControlClient</class>
    <class kind="struct">Transaction</class>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_ASK_FOR_DISCOVERY</name>
      <anchorfile>remoteControlClient_8hpp.html</anchorfile>
      <anchor>ad0741f97391604ed5fc0b123deed1ed9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>remotecontrolserver.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>remotecontrolserver_8hpp.html</filename>
    <includes id="ReqestProcessor_8hpp" name="ReqestProcessor.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/ReqestProcessor.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="class">RemoteControlServer</class>
    <class kind="struct">RemoteNodeInformation</class>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_REPEAT_DETAILED_DATA_REQEST</name>
      <anchorfile>remotecontrolserver_8hpp.html</anchorfile>
      <anchor>adef90e04a6512930c33db7cd9d5e6c0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_REPEAT_INITIAL_DATA_REQEST</name>
      <anchorfile>remotecontrolserver_8hpp.html</anchorfile>
      <anchor>ad6a96d2d1c2ba5f07c09ec43ec81db21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_REPEAT_KEEP_ALIVE_REQEST</name>
      <anchorfile>remotecontrolserver_8hpp.html</anchorfile>
      <anchor>a6a35798fcb91a5b11db34f9112701787</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_SWITCH_FROM_INITIAL_TO_DETAILED</name>
      <anchorfile>remotecontrolserver_8hpp.html</anchorfile>
      <anchor>ac628dcfefdf870274aa932ea3b62643b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ReqestProcessor.hpp</name>
    <path>include/os/app/remoteControl/</path>
    <filename>ReqestProcessor_8hpp.html</filename>
    <includes id="rc__dataTypes_8hpp" name="rc_dataTypes.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/rc_dataTypes.hpp</includes>
    <includes id="RC__publicDataTypes_8hpp" name="RC_publicDataTypes.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/RC_publicDataTypes.hpp</includes>
    <includes id="messageUDP_8hpp" name="messageUDP.hpp" local="no" import="no" module="no" objc="no">os/tools/messageUDP.hpp</includes>
    <class kind="class">RequestProcessor</class>
  </compound>
  <compound kind="file">
    <name>RemoteDevicesManager.hpp</name>
    <path>include/os/app/</path>
    <filename>RemoteDevicesManager_8hpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="struct">RCTranslation</class>
    <class kind="class">RemoteDevicesManager</class>
    <class kind="struct">RemoteDevicesManager::ServiceCallFingerprint</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_EXTERNAL_NODES</name>
      <anchorfile>RemoteDevicesManager_8hpp.html</anchorfile>
      <anchor>a73445eafad9d33ad9874d6d9c48b22df</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>struct</type>
      <name>__attribute__</name>
      <anchorfile>RemoteDevicesManager_8hpp.html</anchorfile>
      <anchor>acc4c6d8dbd002cfe1f76b2ff82e743e3</anchor>
      <arglist>((packed)) ExternalNodeMapping</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>SafeResetManager.hpp</name>
    <path>include/os/app/</path>
    <filename>SafeResetManager_8hpp.html</filename>
    <class kind="class">SafeResetManager</class>
  </compound>
  <compound kind="file">
    <name>timeMaster.hpp</name>
    <path>include/os/app/</path>
    <filename>timeMaster_8hpp.html</filename>
    <includes id="datacontainertypes_8hpp" name="datacontainertypes.hpp" local="no" import="no" module="no" objc="no">os/datacontainer/datacontainertypes.hpp</includes>
    <class kind="class">TimeMaster</class>
  </compound>
  <compound kind="file">
    <name>CyclicProfiler.hpp</name>
    <path>include/os/</path>
    <filename>CyclicProfiler_8hpp.html</filename>
    <class kind="class">CyclicProfiler</class>
    <class kind="struct">CyclicProfiler::Stat</class>
  </compound>
  <compound kind="file">
    <name>datacontainer.hpp</name>
    <path>include/os/datacontainer/</path>
    <filename>datacontainer_8hpp.html</filename>
    <class kind="class">DataContainer</class>
  </compound>
  <compound kind="file">
    <name>datacontainertypes.hpp</name>
    <path>include/os/datacontainer/</path>
    <filename>datacontainertypes_8hpp.html</filename>
    <includes id="DeviceControlApi_8hpp" name="DeviceControlApi.hpp" local="yes" import="no" module="no" objc="no">DeviceControlApi.hpp</includes>
    <includes id="RC__publicDataTypes_8hpp" name="RC_publicDataTypes.hpp" local="yes" import="no" module="no" objc="no">os/app/remoteControl/RC_publicDataTypes.hpp</includes>
    <includes id="NvmConfigSlotDefinition_8hpp" name="NvmConfigSlotDefinition.hpp" local="yes" import="no" module="no" objc="no">os/datacontainer/NvmConfigSlotDefinition.hpp</includes>
    <includes id="messageUDP_8hpp" name="messageUDP.hpp" local="yes" import="no" module="no" objc="no">os/tools/messageUDP.hpp</includes>
    <class kind="struct">ConfigSlotsDataType</class>
    <class kind="struct">DeviceConfigManipulationAPI</class>
    <class kind="struct">DeviceServicesAPI</class>
    <class kind="struct">ExtendedMemoryCtrlAPI</class>
    <class kind="class">NetworkNodeInfo</class>
    <class kind="struct">NodeConfiguration</class>
    <class kind="struct">PersistentDataBlock</class>
    <class kind="class">RFButtonsControlAPI</class>
    <class kind="class">ServiceInformation</class>
    <class kind="struct">SystemErrorType</class>
    <class kind="struct">UINotificationsControlAPI</class>
    <class kind="struct">UserInterfaceNotification</class>
    <namespace>DigitalEvent</namespace>
    <member kind="define">
      <type>#define</type>
      <name>NUMBER_OF_CONFIG_SLOTS</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>ac3d98dbef116359490d60108fbc455b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PERSISTENT_DATABLOCK_SIZE</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a762799e1dd238f750906efaf0853812a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>PersistentDatablockID</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_PERSISTENT_BLOCK_FIRST</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a2bebec921baa90c0e3bad1179ce086e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DEVICE_1</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a7742f5cb5202af03c75420a4dd404e4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DEVICE_2</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a347f739dfa9993f10ab5c07f15f87fc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DEVICE_3</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48aab69af4f47d3824e7bf8e232f8dd5144</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DEVICE_4</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a8682417e1553878b19f5157c893dee57</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DEVICE_5</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48aaedc5106c3330bbe887050dbec7c226a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DEVICE_6</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a36a8fcae5e8d3ee7e6f5369487af785f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_FIRST</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48ae839c406e5aadbda17dbc2d2580df85e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_1</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a11d6f7bfa3d9723853dc8cf83d9a0818</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_2</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a95fe329945eaae8b900904836b0ae608</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_3</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a7f693e542b79739e3c44f380e98e9a87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_4</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48aa41276166d1e990da48e79bb364f50b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_5</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a20d69b9b869984feacc71d0fa338d508</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_6</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48ac6798c4d7f3b1623605df0277013bb96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_7</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a3dd321b5e31a8d2804a8ecc986fe2f07</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_8</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a16173b74f0b0acb54729ec36cf1081c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_HTTP_LAST</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48ada3033dd5441b0a23a973eb8ad9e51b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_EXT_MEMORY_METADATA</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a5fdf7fae18a3fe69a891df58593f564e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RDM_1</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a08d97a87cbacaf5206725bc492440a84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RDM_2</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a0759869611e893a8a047861034f206e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RDM_3</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a68a6db775287e112d43f3a7afb57b5e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RDM_4</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a0416feb29fe831460e2ab0765839ef5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RDM_5</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48ad004d3f3b470dd258462c884a1f5b5df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DIGITAL_EVENT_1</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48ac76908a6c88a14348e479f7b7e42a7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DIGITAL_EVENT_2</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a5e550c013d3f8cf6e7c780f9dde18139</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DIGITAL_EVENT_3</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48afcddff64492e12831dc35f345a7e38c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DIGITAL_EVENT_4</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a6815f46766fe3da24a557fdc91a87671</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DIGITAL_EVENT_5</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a4ade37280d5daf387f2a42437170032c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_DIGITAL_EVENT_6</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48add6fe8368ccebf3344c8fca6b83a85d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RFMANAGER_1</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a2731641413c232f306428a09ebdc1030</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_BLOCK_RFMANAGER_2</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a6a883976d46fb7df65b444c6fc77cb5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_PERSISTENT_BLOCK_LAST</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a0a4c8baeebab959d78910990b67b5836</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_NUMBER_OF_PERSISTENT_BLOCKS</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a60fc2e93e5602be5d234f6150170eb48a0da9199ef5c0895e7a8ccc9e80af2dc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>RequestProcessingState</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a056874975fd16fb39bf4dd639a675762</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>eNO_REQUEST</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a056874975fd16fb39bf4dd639a675762a6a02bcef1619f0d00896705a50f8481d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>eREQUEST_PENDING</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a056874975fd16fb39bf4dd639a675762ae587f33ddbccc13af64207635730a881</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>eREQUEST_COMPLETED</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a056874975fd16fb39bf4dd639a675762a94e8f6cde06dd46e2ec475dbcbb5d82c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>SecurityAccessLevelType</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a50a624b41b1ab08096f2ebc8e680327b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_ACCESS_LEVEL_NONE</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a50a624b41b1ab08096f2ebc8e680327bac330a5b50b92f8db02a425bc3afcf8b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_ACCESS_LEVEL_SERVICE_MODE</name>
      <anchorfile>datacontainertypes_8hpp.html</anchorfile>
      <anchor>a50a624b41b1ab08096f2ebc8e680327ba8c1e96ed57cd344aafeb89ec8eee0c2b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DeviceControlApi.hpp</name>
    <path>include/os/datacontainer/</path>
    <filename>DeviceControlApi_8hpp.html</filename>
    <class kind="struct">DeviceControlFunctionSet</class>
  </compound>
  <compound kind="file">
    <name>NvmConfigSlotDefinition.hpp</name>
    <path>include/os/datacontainer/</path>
    <filename>NvmConfigSlotDefinition_8hpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <class kind="class">DeviceConfigSlotType</class>
  </compound>
  <compound kind="file">
    <name>sigmessages.hpp</name>
    <path>include/os/datacontainer/</path>
    <filename>sigmessages_8hpp.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>ClientState</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>a8d4a6786d6193f0e6245c44e4a7bb4a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_SLAVE_KEEP_ALIVE</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>a8d4a6786d6193f0e6245c44e4a7bb4a0ab83f6018c32e525eaf878a8fed0559a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_NODE_INITIAL_DATA</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>a8d4a6786d6193f0e6245c44e4a7bb4a0a31e8173de4b9f070ab9d63d995b2f07a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_NODE_DETAILED_DATA</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>a8d4a6786d6193f0e6245c44e4a7bb4a0a57945b3af0bae1b391565ce30a65bf00</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ServerState</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>aeee15ce451435cc7f2fdb211337d0694</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_REQUEST_NODE_INITIAL_DATA</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>aeee15ce451435cc7f2fdb211337d0694acd4221911e5c1bc208aa554e3272af1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_REQUEST_NODE_DETAILED_DATA</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>aeee15ce451435cc7f2fdb211337d0694a8752cffbc773c600cb2e0374195478aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_KEEP_ALIVE</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>aeee15ce451435cc7f2fdb211337d0694aae6a9e17a5e72a3d4873ce5fe8597cff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STATE_INVALID</name>
      <anchorfile>sigmessages_8hpp.html</anchorfile>
      <anchor>aeee15ce451435cc7f2fdb211337d0694a56285fc9e39dbb4dd2f0a34e561a79fd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>signals.hpp</name>
    <path>include/os/datacontainer/</path>
    <filename>signals_8hpp.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>NUMBER_OF_SIGNALS</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>ade0778ec1c851612f6cfde13a54ebbca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>Signal</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_KEEP_ALIVE_RECEIVED</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea1c7559649ab2d9127c3bd0cb475b4a90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_CONNECTION_STATUS</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea716bdf97f9065e72b97fac3afd4c4f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_LOCAL_COLLECTION</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea13a361b7b84388bbc3cd4103f4c0ee28</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_DEVICE_COLLECTION</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eaae4d5a14fbe8a98a5207f275a81a0fae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_REMOTE_COLLECTION</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eabed106ab33a377efbe2b017251dede66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_IS_HTTP_SERVER</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea028b2afe3f411ded1d257238dddae0aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_IS_RC_SERVER</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea297ee0f702f7750e32adf1d30e8e6b24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_DEVICE_CONFIGURATION</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eab3ea8a2d4c5eb29bc569289e30bf7e19</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_SET_CONFIG_VIA_JSON_STRING</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eadb0bf6f5869cfc60a32feddc361bc2bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_IP_ADDRESS_STRING</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea75742edaa6eb8c92d22ddf764847331c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_IP_ADDRESS</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea29c9ad2ec99eb2369fb6d17730bf536d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_SET_NVM_DATABLOCK</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea3d84bcadd5eab4f1a585cef46e373628</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_GET_NVM_DATABLOCK</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea40789f2e4205ea6f804431bbe0c56358</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_CONFIG_SLOTS</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea1d6a7568b213a16fd005bbc1b4d3180a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_RESET_DEVICE</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eaeb4e40a67242c9c54abeb56b860feda6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_SYSTEM_ERROR_LIST</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eabaede36f7ae5cd914e8f2a60e210b71d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_MASS_ERASE</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea3a4e2bc6e98437bdea6986ba62e16a9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_REGISTER_REQUEST_RECEIVER</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea57609be895d5c54be07682d7dc6ae1ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_REGISTER_RESPONSE_RECEIVER</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea713b8e25f31322419652efd941c6d8cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_RESPONSE</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eadeba2ce84ba10cf00d6af990a3518abd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_RUNTIME_NODE_HASH</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea878ad8d05fc27633fcc98ab9a902c0bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_RC_DEVICES_INTERNAL_TUNNEL</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea4adddf0222d141c99f2578026127347a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_CREATE_RC_REQUEST</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eaec359b916197ca7e9d282d946a67f600</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_LOCAL_DEVICE_SERVICES</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eaa178f33c637d3e312522ffb0ae238de6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_REMOTE_DEVICE_SERVICES</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea8c5c83a683892533a7e67eb380f1846d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_DEVICE_SERVICES</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea850ce78a927b501182f6620da264198e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_SECURITY_ACCESS_LEVEL</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea4c08baaabe45f2773646474e33fe4a6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_SECURITY_ACCESS_LEVEL_CHANGE_VIA_STRING</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea6d5144e1985f69b44bbc649dfe37d1dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_MAC_ADDRESS</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea4f7107093b28ab5fc3e14cbcd7c99d52</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_UI_NOTIFICATIONS_CONTROL</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea3b3d6d44ca87675349e3ccc8525a335d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_CALCULATE_RUNTIME_NODE_HASH</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea6fa5fa5f255d76cbfee23eb005a31cbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_UPDATE_RC_SLAVE_INFORMATION</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea4aec0fe6fea5b13c8d54c41056452428</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_EXT_MEM_CTRL_API</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea0d830108d5866aa5900134de2de859df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_SET_DEVICES_CONFIG_VIA_JSON</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea7f0dc912f0ac3e659b1663726edc24a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_GET_CURRENT_TIME</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea9c9e34c016050d526858df43dd8631b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_GET_ROOMS_CFG_JSON</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea0429d7ea46daa9aa802effef5eeb264d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_DISPLAY_RAM_USAGE</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eae14ce9c1276c562dfc402de43858a6ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_NETWORK_NODES_INFO</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea3f610f27759898a0e1c1d86b7e4c3800</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_START_NVM_SAVE_TIMER</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eab6dd2f7e838cc0169968ed557cc5213a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_STARTUP_TIME</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea17476a9c5723bfb2c89fbd0b861c1adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_FIRE_DIGITAL_EVENT</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea20a1946b6b1ceceddac02ab6b6a80e7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_DIGITAL_EVNT_MAPPING</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea5e2c1d9472ba0dbe5614e938888cbc7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_UPDATE_DIG_EVNT_TABLE</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829eadc891e1c64c69f16b2502c87dabce26f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_RF_BUTTONS_CTRL_API</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea6724c839196527c8bdfe45d65472b16d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_LAST_KNOWN_MASTER_IP_ADDR</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea56e98ad4a188c9e4e5ecffe071e6b205</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CBK_RECONNECT_WIFI</name>
      <anchorfile>signals_8hpp.html</anchorfile>
      <anchor>af01fe53890022ebf14996ce6824f829ea41907f4618e5d24a7f60cfeb6bb6967c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>networkdriver.hpp</name>
    <path>include/os/drivers/</path>
    <filename>networkdriver_8hpp.html</filename>
    <class kind="class">NetworkDriver</class>
    <member kind="enumeration">
      <type></type>
      <name>PacketRangeDefinition</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>a530b49f249ea1e7de1ef59dde2f7ec45</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVICE_RANGE_BEGIN</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>a530b49f249ea1e7de1ef59dde2f7ec45a66b975e4909ef43073990d104540c730</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RCS_RANGE_BEGIN</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>a530b49f249ea1e7de1ef59dde2f7ec45a2471ae5dbbeb3e68a65bfce6b0e0f841</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DIGITAL_BUTTON_RANGE</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>a530b49f249ea1e7de1ef59dde2f7ec45a8ee8947465fb4c9476f998a46225cd88</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DISPLAY_CONTROLS_RANGE</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>a530b49f249ea1e7de1ef59dde2f7ec45a164436b68f2d9efd37c6f6ea22283b79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>USR_DATA_RANGE_BEGIN</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>a530b49f249ea1e7de1ef59dde2f7ec45a50311296248ba8ccb5e7197d1442dbd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const MessageUDP::IPAddr</type>
      <name>NETWORK_BROADCAST</name>
      <anchorfile>networkdriver_8hpp.html</anchorfile>
      <anchor>ad162c0062351c96ed6c183eacaa6fcf8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ota.hpp</name>
    <path>include/os/drivers/</path>
    <filename>ota_8hpp.html</filename>
    <class kind="class">OTA</class>
  </compound>
  <compound kind="file">
    <name>HomeStation_os.hpp</name>
    <path>include/os/</path>
    <filename>HomeStation__os_8hpp.html</filename>
    <includes id="CyclicProfiler_8hpp" name="CyclicProfiler.hpp" local="yes" import="no" module="no" objc="no">os/CyclicProfiler.hpp</includes>
    <includes id="RemoteDevicesManager_8hpp" name="RemoteDevicesManager.hpp" local="no" import="no" module="no" objc="no">os/app/RemoteDevicesManager.hpp</includes>
    <includes id="NotificationHandler_8hpp" name="NotificationHandler.hpp" local="no" import="no" module="no" objc="no">os/app/NotificationHandler.hpp</includes>
    <includes id="DigitalEventReceiver_8hpp" name="DigitalEventReceiver.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventReceiver.hpp</includes>
    <includes id="DigitalEventTransmitter_8hpp" name="DigitalEventTransmitter.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventTransmitter.hpp</includes>
    <includes id="ExtendedMemoryManager_8hpp" name="ExtendedMemoryManager.hpp" local="no" import="no" module="no" objc="no">os/app/config/ExtendedMemoryManager.hpp</includes>
    <includes id="SafeResetManager_8hpp" name="SafeResetManager.hpp" local="no" import="no" module="no" objc="no">os/app/SafeResetManager.hpp</includes>
    <includes id="timeMaster_8hpp" name="timeMaster.hpp" local="no" import="no" module="no" objc="no">os/app/timeMaster.hpp</includes>
    <includes id="DisplayServer_8hpp" name="DisplayServer.hpp" local="no" import="no" module="no" objc="no">os/app/display/DisplayServer.hpp</includes>
    <includes id="RFManager_8hpp" name="RFManager.hpp" local="no" import="no" module="no" objc="no">os/HwDependency/RFManager.hpp</includes>
    <class kind="class">OperatingSystem</class>
  </compound>
  <compound kind="file">
    <name>RFManager.hpp</name>
    <path>include/os/HwDependency/</path>
    <filename>RFManager_8hpp.html</filename>
    <includes id="SystemDefinition_8hpp" name="SystemDefinition.hpp" local="yes" import="no" module="no" objc="no">SystemDefinition.hpp</includes>
    <includes id="datacontainertypes_8hpp" name="datacontainertypes.hpp" local="yes" import="no" module="no" objc="no">os/datacontainer/datacontainertypes.hpp</includes>
    <class kind="struct">RFButtonInfo</class>
    <class kind="class">RFManager</class>
    <class kind="struct">RFReceptionEvent</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_RF_BUTTONS_STORED</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>aba960c886ca4fffaaf39d7aea9e914c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RF_BUTTONS_CLICK_WHILE_LEARNING</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>ad5432479ea63cf0995b1525c9d236aad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_BETWEEN_EVENTS_MS</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>a4b39398ea3a00f44df2c3c98ea5cead4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_FOR_LONG_PRESS_MS</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>a27e850e52e172ed15fd74085697b5236</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_COMPLETE_EVENT</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>a7d2e6e8780440e0f6817bb5b4150d3d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMEOUT_RF_BUTTON_LEARNING_MS</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>af067caf782d53236ca56a068bb6baf56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>RFManagerState</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>ab3164591a432a0ee3c8559e0a5e77c6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_RF_BUTTON_LEARNING</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>ab3164591a432a0ee3c8559e0a5e77c6cac755c92dcdce948307166b5e2081f3b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>e_RF_IDLE</name>
      <anchorfile>RFManager_8hpp.html</anchorfile>
      <anchor>ab3164591a432a0ee3c8559e0a5e77c6ca4f61a6cc0a665817329b0d270dd6790d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Logger.hpp</name>
    <path>include/os/</path>
    <filename>Logger_8hpp.html</filename>
    <class kind="class">Logger</class>
  </compound>
  <compound kind="file">
    <name>ExtendedDataAllocator.hpp</name>
    <path>include/os/tools/</path>
    <filename>ExtendedDataAllocator_8hpp.html</filename>
    <includes id="ExtendedDataTypes_8hpp" name="ExtendedDataTypes.hpp" local="no" import="no" module="no" objc="no">os/tools/ExtendedDataTypes.hpp</includes>
    <class kind="class">ExtendedDataAllocator</class>
  </compound>
  <compound kind="file">
    <name>ExtendedDataTypes.hpp</name>
    <path>include/os/tools/</path>
    <filename>ExtendedDataTypes_8hpp.html</filename>
    <class kind="struct">ExtendedData</class>
  </compound>
  <compound kind="file">
    <name>messageUDP.hpp</name>
    <path>include/os/tools/</path>
    <filename>messageUDP_8hpp.html</filename>
    <class kind="struct">MessageUDP::IPAddr</class>
    <class kind="class">MessageUDP</class>
  </compound>
  <compound kind="file">
    <name>udpAdapter.hpp</name>
    <path>include/os/tools/</path>
    <filename>udpAdapter_8hpp.html</filename>
    <class kind="class">UDPAdapter</class>
  </compound>
  <compound kind="file">
    <name>wifiAdapter.hpp</name>
    <path>include/os/tools/</path>
    <filename>wifiAdapter_8hpp.html</filename>
    <class kind="class">WiFiAdapter</class>
    <member kind="enumeration">
      <type></type>
      <name>NetworkStatus</name>
      <anchorfile>wifiAdapter_8hpp.html</anchorfile>
      <anchor>afbfc456845719ac215946bb2b05eb2df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ACCESS_POINT_MODE</name>
      <anchorfile>wifiAdapter_8hpp.html</anchorfile>
      <anchor>afbfc456845719ac215946bb2b05eb2dfa5d91f8651560e2e6565fb45773275d39</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CONNECTED_TO_NETWORK</name>
      <anchorfile>wifiAdapter_8hpp.html</anchorfile>
      <anchor>afbfc456845719ac215946bb2b05eb2dfacd59ac17132909f1877e6babce875b84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DISCONNECTED_FROM_NETWORK</name>
      <anchorfile>wifiAdapter_8hpp.html</anchorfile>
      <anchor>afbfc456845719ac215946bb2b05eb2dfad9a05bcd4d72156df90ad3e711397c45</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>SystemDefinition.hpp</name>
    <path>include/</path>
    <filename>SystemDefinition_8hpp.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DISTANCE_SENSOR_SUPPORTED</name>
      <anchorfile>SystemDefinition_8hpp.html</anchorfile>
      <anchor>a66ede986f779fab416e5bae8b5cfd3ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HAS_RF_RECEIVER</name>
      <anchorfile>SystemDefinition_8hpp.html</anchorfile>
      <anchor>a50af5e7bf7d24884ad9fb627d49c6286</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_STRIP_SUPPORTED</name>
      <anchorfile>SystemDefinition_8hpp.html</anchorfile>
      <anchor>ae311a3eb60574cf3cf91deaea54d3962</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ON_OFF_SUPPORTED</name>
      <anchorfile>SystemDefinition_8hpp.html</anchorfile>
      <anchor>a88861a001338f4731b0f34b9aeed36bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TEMP_SENSOR_SUPPORTED</name>
      <anchorfile>SystemDefinition_8hpp.html</anchorfile>
      <anchor>ae9be72519766cdb60a4c21fa0e41855d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>build_info.h</name>
    <path>src/</path>
    <filename>build__info_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>BUILD_GITHASH</name>
      <anchorfile>build__info_8h.html</anchorfile>
      <anchor>aa8c8b367a31a1e0e7c86c7d3ddcead66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BUILD_TIMESTAMP</name>
      <anchorfile>build__info_8h.html</anchorfile>
      <anchor>a50e40f98b0a2e1338e46f50d178434c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEVELOPER</name>
      <anchorfile>build__info_8h.html</anchorfile>
      <anchor>a48e4b155916c5a6b0f861fb825d08f7c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HwButton.cpp</name>
    <path>src/device/CustomHardware/</path>
    <filename>HwButton_8cpp.html</filename>
    <includes id="HwButton_8hpp" name="HwButton.hpp" local="yes" import="no" module="no" objc="no">devices/CustomHardware/HwButton.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>distanceSensor.cpp</name>
    <path>src/device/</path>
    <filename>distanceSensor_8cpp.html</filename>
    <includes id="distanceSensor_8hpp" name="distanceSensor.hpp" local="yes" import="no" module="no" objc="no">devices/distanceSensor.hpp</includes>
  </compound>
  <compound kind="file">
    <name>BounceInAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>BounceInAnimation_8cpp.html</filename>
    <includes id="BounceInAnimation_8hpp" name="BounceInAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/BounceInAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>FadeIn.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>FadeIn_8cpp.html</filename>
    <includes id="FadeIn_8hpp" name="FadeIn.hpp" local="no" import="no" module="no" objc="no">devices/LedStrip/Animations/FadeIn.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_INCR_STEP</name>
      <anchorfile>FadeIn_8cpp.html</anchorfile>
      <anchor>a574b2a483a00542860db0995ee418f5d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>FadeOut.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>FadeOut_8cpp.html</filename>
    <includes id="FadeOut_8hpp" name="FadeOut.hpp" local="no" import="no" module="no" objc="no">devices/LedStrip/Animations/FadeOut.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_INCR_STEP</name>
      <anchorfile>FadeOut_8cpp.html</anchorfile>
      <anchor>a574b2a483a00542860db0995ee418f5d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ILedAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>ILedAnimation_8cpp.html</filename>
    <includes id="ILedAnimation_8hpp" name="ILedAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/ILedAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>ILiveAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>ILiveAnimation_8cpp.html</filename>
  </compound>
  <compound kind="file">
    <name>SingleWaveAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/live/</path>
    <filename>SingleWaveAnimation_8cpp.html</filename>
  </compound>
  <compound kind="file">
    <name>RollInAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>RollInAnimation_8cpp.html</filename>
    <includes id="RollInAnimation_8hpp" name="RollInAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/RollInAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>RollOutAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>RollOutAnimation_8cpp.html</filename>
    <includes id="RollOutAnimation_8hpp" name="RollOutAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/RollOutAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>SparkleInAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>SparkleInAnimation_8cpp.html</filename>
    <includes id="SparkleInAnimation_8hpp" name="SparkleInAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/SparkleInAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>TwinkleInAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>TwinkleInAnimation_8cpp.html</filename>
    <includes id="TwinkleInAnimation_8hpp" name="TwinkleInAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/TwinkleInAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>WaveInAnimation.cpp</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>WaveInAnimation_8cpp.html</filename>
    <includes id="WaveInAnimation_8hpp" name="WaveInAnimation.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Animations/WaveInAnimation.hpp</includes>
  </compound>
  <compound kind="file">
    <name>LedWS1228b.cpp</name>
    <path>src/device/LedStrip/</path>
    <filename>LedWS1228b_8cpp.html</filename>
    <includes id="LedWS1228b_8hpp" name="LedWS1228b.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/LedWS1228b.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="function">
      <type>bool</type>
      <name>isNotBlack</name>
      <anchorfile>LedWS1228b_8cpp.html</anchorfile>
      <anchor>ada9040e09bb96a0b1314ed0624b067a3</anchor>
      <arglist>(LedColor color)</arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>maxVirtualLeds</name>
      <anchorfile>LedWS1228b_8cpp.html</anchorfile>
      <anchor>a4cc7b20a1d6df81fda4aae1caeee2347</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Segmented_LedWS1228b.cpp</name>
    <path>src/device/LedStrip/</path>
    <filename>Segmented__LedWS1228b_8cpp.html</filename>
    <includes id="Segmented__LedWS1228b_8hpp" name="Segmented_LedWS1228b.hpp" local="yes" import="no" module="no" objc="no">devices/LedStrip/Segmented_LedWS1228b.hpp</includes>
    <member kind="function">
      <type>bool</type>
      <name>notBlack</name>
      <anchorfile>Segmented__LedWS1228b_8cpp.html</anchorfile>
      <anchor>ae9fb6df8ad536d6741da534d26bc11b2</anchor>
      <arglist>(LedColor color)</arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>maxVirtualLeds</name>
      <anchorfile>Segmented__LedWS1228b_8cpp.html</anchorfile>
      <anchor>a4cc7b20a1d6df81fda4aae1caeee2347</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>onoffdevice.cpp</name>
    <path>src/device/</path>
    <filename>onoffdevice_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>tempSensorDHT11.cpp</name>
    <path>src/device/</path>
    <filename>tempSensorDHT11_8cpp.html</filename>
    <includes id="tempSensorDHT11_8hpp" name="tempSensorDHT11.hpp" local="yes" import="no" module="no" objc="no">devices/tempSensorDHT11.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>TestDeviceType.cpp</name>
    <path>src/device/</path>
    <filename>TestDeviceType_8cpp.html</filename>
    <includes id="TestDeviceType_8hpp" name="TestDeviceType.hpp" local="yes" import="no" module="no" objc="no">devices/TestDeviceType.hpp</includes>
  </compound>
  <compound kind="file">
    <name>main.cpp</name>
    <path>src/</path>
    <filename>main_8cpp.html</filename>
    <includes id="HomeStation__os_8hpp" name="HomeStation_os.hpp" local="no" import="no" module="no" objc="no">os/HomeStation_os.hpp</includes>
    <includes id="CyclicProfiler_8hpp" name="CyclicProfiler.hpp" local="no" import="no" module="no" objc="no">os/CyclicProfiler.hpp</includes>
    <member kind="function">
      <type>void</type>
      <name>loop</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>afe461d27b9c48d5921c00d521181f12f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setup</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a4fc01d736fe50cf5b977f755b675f11d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>configprovider.cpp</name>
    <path>src/os/app/config/</path>
    <filename>configprovider_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <includes id="datacontainertypes_8hpp" name="datacontainertypes.hpp" local="yes" import="no" module="no" objc="no">os/datacontainer/datacontainertypes.hpp</includes>
    <member kind="function">
      <type>bool</type>
      <name>isNotNull</name>
      <anchorfile>configprovider_8cpp.html</anchorfile>
      <anchor>a64d9bec3cd2c9460ef7ba5d358d7aa85</anchor>
      <arglist>(const String &amp;str)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ExtendedMemoryManager.cpp</name>
    <path>src/os/app/config/</path>
    <filename>ExtendedMemoryManager_8cpp.html</filename>
    <includes id="ExtendedMemoryManager_8hpp" name="ExtendedMemoryManager.hpp" local="no" import="no" module="no" objc="no">os/app/config/ExtendedMemoryManager.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>persistentMemoryAccess.cpp</name>
    <path>src/os/app/config/</path>
    <filename>persistentMemoryAccess_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>END_OF_DATA</name>
      <anchorfile>persistentMemoryAccess_8cpp.html</anchorfile>
      <anchor>af62f31e4727cb54bee2518937fd33e6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>START_OF_DATA</name>
      <anchorfile>persistentMemoryAccess_8cpp.html</anchorfile>
      <anchor>a420fd99cec11087e957e77ab1e852eb8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>devicemanager.cpp</name>
    <path>src/os/app/</path>
    <filename>devicemanager_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="no" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="function">
      <type>bool</type>
      <name>isNull</name>
      <anchorfile>devicemanager_8cpp.html</anchorfile>
      <anchor>a9f22d9639a3869473f0e87723b50bc21</anchor>
      <arglist>(const String &amp;str)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>deviceProvider.cpp</name>
    <path>src/os/app/</path>
    <filename>deviceProvider_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="no" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>MINIMAL_PAYLOAD_SIZE</name>
      <anchorfile>deviceProvider_8cpp.html</anchorfile>
      <anchor>a23f1cd392b9a983907a3cea8c39f22bc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DigitalEventReceiver.cpp</name>
    <path>src/os/app/DigitalEvent/</path>
    <filename>DigitalEventReceiver_8cpp.html</filename>
    <includes id="DigitalEventReceiver_8hpp" name="DigitalEventReceiver.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventReceiver.hpp</includes>
    <includes id="networkdriver_8hpp" name="networkdriver.hpp" local="no" import="no" module="no" objc="no">os/drivers/networkdriver.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>deviceOrRoomIdIndex</name>
      <anchorfile>DigitalEventReceiver_8cpp.html</anchorfile>
      <anchor>a295ea2d1010ab0abb3d243f6ca259155</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>NVM_VALID</name>
      <anchorfile>DigitalEventReceiver_8cpp.html</anchorfile>
      <anchor>ac4659bb02dcb2db7ca4075eb64192741</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>serviceOverloadingIndex</name>
      <anchorfile>DigitalEventReceiver_8cpp.html</anchorfile>
      <anchor>a6612832ff9f915545226a29da821e884</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>serviceTypeIndex</name>
      <anchorfile>DigitalEventReceiver_8cpp.html</anchorfile>
      <anchor>a0cd17db166f3d8991e3f34c085b2107e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>valueIndex</name>
      <anchorfile>DigitalEventReceiver_8cpp.html</anchorfile>
      <anchor>a1f856fbfba41e80f197503fd6f2db9c7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DigitalEventTransmitter.cpp</name>
    <path>src/os/app/DigitalEvent/</path>
    <filename>DigitalEventTransmitter_8cpp.html</filename>
    <includes id="DigitalEventTransmitter_8hpp" name="DigitalEventTransmitter.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventTransmitter.hpp</includes>
    <includes id="networkdriver_8hpp" name="networkdriver.hpp" local="no" import="no" module="no" objc="no">os/drivers/networkdriver.hpp</includes>
  </compound>
  <compound kind="file">
    <name>DeviceDescriptionPacker.cpp</name>
    <path>src/os/app/display/</path>
    <filename>DeviceDescriptionPacker_8cpp.html</filename>
    <includes id="DeviceDescriptionPacker_8hpp" name="DeviceDescriptionPacker.hpp" local="yes" import="no" module="no" objc="no">os/app/display/DeviceDescriptionPacker.hpp</includes>
  </compound>
  <compound kind="file">
    <name>Display.cpp</name>
    <path>src/os/app/display/</path>
    <filename>Display_8cpp.html</filename>
    <includes id="Display_8hpp" name="Display.hpp" local="yes" import="no" module="no" objc="no">os/app/display/Display.hpp</includes>
  </compound>
  <compound kind="file">
    <name>DisplayServer.cpp</name>
    <path>src/os/app/display/</path>
    <filename>DisplayServer_8cpp.html</filename>
    <includes id="DisplayServer_8hpp" name="DisplayServer.hpp" local="yes" import="no" module="no" objc="no">os/app/display/DisplayServer.hpp</includes>
    <includes id="networkdriver_8hpp" name="networkdriver.hpp" local="yes" import="no" module="no" objc="no">os/drivers/networkdriver.hpp</includes>
  </compound>
  <compound kind="file">
    <name>AdvancedControlsLoader.cpp</name>
    <path>src/os/app/http/</path>
    <filename>AdvancedControlsLoader_8cpp.html</filename>
    <includes id="AdvancedControlsLoader_8hpp" name="AdvancedControlsLoader.hpp" local="no" import="no" module="no" objc="no">os/app/http/AdvancedControlsLoader.hpp</includes>
    <includes id="SystemDefinition_8hpp" name="SystemDefinition.hpp" local="no" import="no" module="no" objc="no">SystemDefinition.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>ONOFF_ADV_CONTROLS_SIZE</name>
      <anchorfile>AdvancedControlsLoader_8cpp.html</anchorfile>
      <anchor>a6f63db7bcb6891f4dc9f7cd5c41329db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TEMPSENSOR_ADV_CONTROLS_SIZE</name>
      <anchorfile>AdvancedControlsLoader_8cpp.html</anchorfile>
      <anchor>a11903c64c0c25e2f356baa55be7af097</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HttpAsyncRequestHandler.cpp</name>
    <path>src/os/app/http/</path>
    <filename>HttpAsyncRequestHandler_8cpp.html</filename>
    <includes id="HttpAsyncRequestHandler_8hpp" name="HttpAsyncRequestHandler.hpp" local="no" import="no" module="no" objc="no">os/app/http/HttpAsyncRequestHandler.hpp</includes>
    <includes id="AdvancedControlsLoader_8hpp" name="AdvancedControlsLoader.hpp" local="no" import="no" module="no" objc="no">os/app/http/AdvancedControlsLoader.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="function">
      <type>String</type>
      <name>getHexColor</name>
      <anchorfile>HttpAsyncRequestHandler_8cpp.html</anchorfile>
      <anchor>a8205a4dfefe84a0559ac364389d96035</anchor>
      <arglist>(uint8_t r, uint8_t g, uint8_t b)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>httpserver.cpp</name>
    <path>src/os/app/http/</path>
    <filename>httpserver_8cpp.html</filename>
    <includes id="tempGaugeJS_8h" name="tempGaugeJS.h" local="yes" import="no" module="no" objc="no">os/app/http/tempGauge/tempGaugeJS.h</includes>
    <includes id="tempGaugeCSS_8h" name="tempGaugeCSS.h" local="yes" import="no" module="no" objc="no">os/app/http/tempGauge/tempGaugeCSS.h</includes>
    <includes id="renderRoomsJS_8h" name="renderRoomsJS.h" local="yes" import="no" module="no" objc="no">os/app/http/renderRoomsJS.h</includes>
    <includes id="build__info_8h" name="build_info.h" local="yes" import="no" module="no" objc="no">build_info.h</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="function">
      <type>void</type>
      <name>generateExtraFieldsForDistanceSensor</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>af80ffc6adf99bf5d42403333fb691312</anchor>
      <arglist>(uint8_t slotNumber, DeviceConfigSlotType &amp;slot, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>generateExtraFieldsForLedStrip</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a2ebd564a494c781d7206898287f302bb</anchor>
      <arglist>(uint8_t slotNumber, DeviceConfigSlotType &amp;slot, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>generateExtraFieldsForOnOff</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a17f8d040466fef820afdbb8c60e87b73</anchor>
      <arglist>(uint8_t slotNumber, DeviceConfigSlotType &amp;slot, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>generateExtraFieldsForSegmentedLedStrip</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>ac3b584a475ede4889b9ebbc624afe8fa</anchor>
      <arglist>(uint8_t slotNumber, DeviceConfigSlotType &amp;slot, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="variable">
      <type>WiFiClient</type>
      <name>client</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a323dfc205549b3c13f0a25c1b670708a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; std::pair&lt; std::function&lt; void(WiFiClient &amp;)&gt;, SecurityAccessLevelType &gt; &gt;</type>
      <name>constantRequestHandlers</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a95beeebfd33da0e0d4484205e59d222c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; String &gt;</type>
      <name>constantRequests</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a5c23bd2a6b5850bc93d25bb7a6544081</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>labelEnd</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>ab9c16b336da1a117e039d81e188db042</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>labelStart</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a7d8b6ce95cc77dd5e9ce3abf7495bc75</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; std::pair&lt; std::function&lt; void(String &amp;, WiFiClient &amp;)&gt;, SecurityAccessLevelType &gt; &gt;</type>
      <name>parameterizedAsyncRequestHandlers</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a608bd963d5fc1b1c0fc4bd7baef029e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; String &gt;</type>
      <name>parameterizedAsyncRequests</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>afc672aa5457a9e92544a596c32afeec8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; std::pair&lt; std::function&lt; void(String &amp;, WiFiClient &amp;)&gt;, SecurityAccessLevelType &gt; &gt;</type>
      <name>parameterizedRequestHandlers</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a796d0c12c5c8e57df4b3fb0a40374276</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; String &gt;</type>
      <name>parameterizedRequests</name>
      <anchorfile>httpserver_8cpp.html</anchorfile>
      <anchor>a9ab99f815a2a89099eeb2d9a899315c2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>asyncHandlers.cpp</name>
    <path>src/os/app/http/requestCallbacks/</path>
    <filename>asyncHandlers_8cpp.html</filename>
    <includes id="httpserver_8hpp" name="httpserver.hpp" local="no" import="no" module="no" objc="no">os/app/http/httpserver.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>constantHandlers.cpp</name>
    <path>src/os/app/http/requestCallbacks/</path>
    <filename>constantHandlers_8cpp.html</filename>
    <includes id="httpserver_8hpp" name="httpserver.hpp" local="no" import="no" module="no" objc="no">os/app/http/httpserver.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>parameterizedHandlers.cpp</name>
    <path>src/os/app/http/requestCallbacks/</path>
    <filename>parameterizedHandlers_8cpp.html</filename>
    <includes id="httpserver_8hpp" name="httpserver.hpp" local="no" import="no" module="no" objc="no">os/app/http/httpserver.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>RFManager.cpp</name>
    <path>src/os/app/HwDependency/</path>
    <filename>RFManager_8cpp.html</filename>
    <includes id="RFManager_8hpp" name="RFManager.hpp" local="yes" import="no" module="no" objc="no">os/HwDependency/RFManager.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>NVM_DATA_VALID_FLAG</name>
      <anchorfile>RFManager_8cpp.html</anchorfile>
      <anchor>a2985a5aa143269c6c16fd44a580e7781</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RCSwitch</type>
      <name>mySwitch</name>
      <anchorfile>RFManager_8cpp.html</anchorfile>
      <anchor>af4559a6ea5d8d507f2410260c7f3b746</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>RX_PIN</name>
      <anchorfile>RFManager_8cpp.html</anchorfile>
      <anchor>a35013ede2afec76d0f6952c8b027da4e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>NotificationHandler.cpp</name>
    <path>src/os/app/</path>
    <filename>NotificationHandler_8cpp.html</filename>
    <includes id="NotificationHandler_8hpp" name="NotificationHandler.hpp" local="no" import="no" module="no" objc="no">os/app/NotificationHandler.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>rcRequest.cpp</name>
    <path>src/os/app/remoteControl/</path>
    <filename>rcRequest_8cpp.html</filename>
    <includes id="rcRequest_8hpp" name="rcRequest.hpp" local="yes" import="no" module="no" objc="no">os/app/remoteControl/rcRequest.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>rcResponse.cpp</name>
    <path>src/os/app/remoteControl/</path>
    <filename>rcResponse_8cpp.html</filename>
    <includes id="rcResponse_8hpp" name="rcResponse.hpp" local="yes" import="no" module="no" objc="no">os/app/remoteControl/rcResponse.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>remoteControlClient.cpp</name>
    <path>src/os/app/remoteControl/</path>
    <filename>remoteControlClient_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="variable" static="yes">
      <type>static ClientState</type>
      <name>currentState</name>
      <anchorfile>remoteControlClient_8cpp.html</anchorfile>
      <anchor>af51995f443c40dc0025d51a68771f12d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>remotecontrolserver.cpp</name>
    <path>src/os/app/remoteControl/</path>
    <filename>remotecontrolserver_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>KEEP_ALIVE_INTERVAL</name>
      <anchorfile>remotecontrolserver_8cpp.html</anchorfile>
      <anchor>ae8069406cfdee5772876890b8b839b1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIME_TO_REPEAT_DISCOVERY_REQUEST</name>
      <anchorfile>remotecontrolserver_8cpp.html</anchorfile>
      <anchor>a6014fe7bb84fa822fbe445b73d1c7752</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>initialDataExitTimer</name>
      <anchorfile>remotecontrolserver_8cpp.html</anchorfile>
      <anchor>ac08a656efb5b854c1e18b9dd932fe63d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>requestDetailedDataTimer</name>
      <anchorfile>remotecontrolserver_8cpp.html</anchorfile>
      <anchor>a59389e6164ad49e773379bc10d1c4bc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>requestInitialDataTimer</name>
      <anchorfile>remotecontrolserver_8cpp.html</anchorfile>
      <anchor>a522e3c2ae572daf6dd5e7f9bba40b7cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>requestKeepAliveTimer</name>
      <anchorfile>remotecontrolserver_8cpp.html</anchorfile>
      <anchor>a61a3e84308498518bde3ed74d10b435e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RequestProcessor.cpp</name>
    <path>src/os/app/remoteControl/</path>
    <filename>RequestProcessor_8cpp.html</filename>
    <includes id="ReqestProcessor_8hpp" name="ReqestProcessor.hpp" local="no" import="no" module="no" objc="no">os/app/remoteControl/ReqestProcessor.hpp</includes>
    <includes id="networkdriver_8hpp" name="networkdriver.hpp" local="no" import="no" module="no" objc="no">os/drivers/networkdriver.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>RemoteDevicesManager.cpp</name>
    <path>src/os/app/</path>
    <filename>RemoteDevicesManager_8cpp.html</filename>
    <includes id="RemoteDevicesManager_8hpp" name="RemoteDevicesManager.hpp" local="no" import="no" module="no" objc="no">os/app/RemoteDevicesManager.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="variable">
      <type>ServiceParameters_set3</type>
      <name>localCopyOfLastActiveRequestParamSet3</name>
      <anchorfile>RemoteDevicesManager_8cpp.html</anchorfile>
      <anchor>a94541d01d9d5eee4a779303224e424a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ServiceOverloadingFunction</type>
      <name>localCopyOfLastOverloading</name>
      <anchorfile>RemoteDevicesManager_8cpp.html</anchorfile>
      <anchor>a006ed96e46b562b28abe57a3330af4bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t</type>
      <name>NVM_VALID_FLAG</name>
      <anchorfile>RemoteDevicesManager_8cpp.html</anchorfile>
      <anchor>a00fda3cce09efe418a41f99796d070c7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>SafeResetManager.cpp</name>
    <path>src/os/app/</path>
    <filename>SafeResetManager_8cpp.html</filename>
    <includes id="SafeResetManager_8hpp" name="SafeResetManager.hpp" local="no" import="no" module="no" objc="no">os/app/SafeResetManager.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>timeMaster.cpp</name>
    <path>src/os/app/</path>
    <filename>timeMaster_8cpp.html</filename>
    <includes id="timeMaster_8hpp" name="timeMaster.hpp" local="no" import="no" module="no" objc="no">os/app/timeMaster.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="define">
      <type>#define</type>
      <name>HALF_NTP_RESYNC_TIME</name>
      <anchorfile>timeMaster_8cpp.html</anchorfile>
      <anchor>a9ea2663be9c8534a11a623b947f1c137</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NTP_RESYNC_TIME</name>
      <anchorfile>timeMaster_8cpp.html</anchorfile>
      <anchor>a4a2db74d80fe4c078efda693684b5b63</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>CyclicProfiler.cpp</name>
    <path>src/os/</path>
    <filename>CyclicProfiler_8cpp.html</filename>
    <includes id="CyclicProfiler_8hpp" name="CyclicProfiler.hpp" local="no" import="no" module="no" objc="no">os/CyclicProfiler.hpp</includes>
    <member kind="typedef">
      <type>void(*)()</type>
      <name>FuncPtr</name>
      <anchorfile>CyclicProfiler_8cpp.html</anchorfile>
      <anchor>ab1e5b4455ceaa343d675447972f41547</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>datacontainer.cpp</name>
    <path>src/os/datacontainer/</path>
    <filename>datacontainer_8cpp.html</filename>
  </compound>
  <compound kind="file">
    <name>networkdriver.cpp</name>
    <path>src/os/drivers/</path>
    <filename>networkdriver_8cpp.html</filename>
    <includes id="DigitalEventReceiver_8hpp" name="DigitalEventReceiver.hpp" local="no" import="no" module="no" objc="no">os/app/DigitalEvent/DigitalEventReceiver.hpp</includes>
    <includes id="DisplayServer_8hpp" name="DisplayServer.hpp" local="no" import="no" module="no" objc="no">os/app/display/DisplayServer.hpp</includes>
    <includes id="ota_8hpp" name="ota.hpp" local="no" import="no" module="no" objc="no">os/drivers/ota.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="function">
      <type>void</type>
      <name>dummyTobeRemoved</name>
      <anchorfile>networkdriver_8cpp.html</anchorfile>
      <anchor>aacd29f8e611a26a8cc2396b5792192a9</anchor>
      <arglist>(MessageUDP &amp;data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ota.cpp</name>
    <path>src/os/drivers/</path>
    <filename>ota_8cpp.html</filename>
    <includes id="ota_8hpp" name="ota.hpp" local="yes" import="no" module="no" objc="no">os/drivers/ota.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>HomeStation_os.cpp</name>
    <path>src/os/</path>
    <filename>HomeStation__os_8cpp.html</filename>
    <includes id="HomeStation__os_8hpp" name="HomeStation_os.hpp" local="no" import="no" module="no" objc="no">os/HomeStation_os.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="no" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="function">
      <type>DHT</type>
      <name>tempSensor</name>
      <anchorfile>HomeStation__os_8cpp.html</anchorfile>
      <anchor>abaccda5e77b56f0819f441a01ad722c4</anchor>
      <arglist>(21, DHT11)</arglist>
    </member>
    <member kind="variable">
      <type>long long</type>
      <name>uiBlockTime</name>
      <anchorfile>HomeStation__os_8cpp.html</anchorfile>
      <anchor>aeb21be3bebaaca6d9ef124751c216ad4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Logger.cpp</name>
    <path>src/os/</path>
    <filename>Logger_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="variable">
      <type>WiFiUDP</type>
      <name>udp</name>
      <anchorfile>Logger_8cpp.html</anchorfile>
      <anchor>a46b48e04b347d8c6d55433b1f1dcc5b1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ExtendedDataAllocator.cpp</name>
    <path>src/os/tools/</path>
    <filename>ExtendedDataAllocator_8cpp.html</filename>
    <includes id="ExtendedDataAllocator_8hpp" name="ExtendedDataAllocator.hpp" local="no" import="no" module="no" objc="no">os/tools/ExtendedDataAllocator.hpp</includes>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>messageUDP.cpp</name>
    <path>src/os/tools/</path>
    <filename>messageUDP_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>udpAdapter.cpp</name>
    <path>src/os/tools/</path>
    <filename>udpAdapter_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
  </compound>
  <compound kind="file">
    <name>wifiAdapter.cpp</name>
    <path>src/os/tools/</path>
    <filename>wifiAdapter_8cpp.html</filename>
    <includes id="Logger_8hpp" name="Logger.hpp" local="yes" import="no" module="no" objc="no">os/Logger.hpp</includes>
    <member kind="variable">
      <type>unsigned long</type>
      <name>lastStatusCheck</name>
      <anchorfile>wifiAdapter_8cpp.html</anchorfile>
      <anchor>a172aa54e28f6d7b797cf6967d0696915</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>nextReconnectAt</name>
      <anchorfile>wifiAdapter_8cpp.html</anchorfile>
      <anchor>a9f3163492c03f675e567407f8f7ab719</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const unsigned long</type>
      <name>reconnectDelayMs</name>
      <anchorfile>wifiAdapter_8cpp.html</anchorfile>
      <anchor>a54e6a922a7a8f8b2b12e3fe7546cd577</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const unsigned long</type>
      <name>statusCheckInterval</name>
      <anchorfile>wifiAdapter_8cpp.html</anchorfile>
      <anchor>ae3c75c48bce94613d3cc20368ca0a48c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AdvancedControlsLoader</name>
    <filename>classAdvancedControlsLoader.html</filename>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>getOutpuJavaScript</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>adb4337ad3909917b7c7cc3e585e488d5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>loadAdvancedControlsToJavaScript</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a0b5f3756997af2b3efd5e05affefad59</anchor>
      <arglist>(uint16_t deviceIdentifier)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint8_t *</type>
      <name>allocateMemoryForControlsBasedOnDeviceType</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a192474537716298f68b1effaee4c1dd1</anchor>
      <arglist>(uint8_t deviceType)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static String</type>
      <name>createJsForLedStrip</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a23f5327b0935416da37c5c40bc3254fa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static String</type>
      <name>createJsForOnOff</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a6bddaaf7d9fa9d02d5bcae0c73ac2c7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>getControlsSizeBasedOnDevType</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a3849b76c3d95cc3f76bc745ff1cf3c63</anchor>
      <arglist>(uint8_t deviceType)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>prepareJsStringWithAdvancedControls</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a1361a42ee70a4fb0017baff2962acb6e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t *</type>
      <name>currentAdvancedControls</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a74e66c963f55d90fe4b822bd68afe3a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static DeviceDescription</type>
      <name>currentlyRequestedDeviceDescription</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a9502095d4d9e355bad25ab26dfd60145</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static String</type>
      <name>currentRequestJS</name>
      <anchorfile>classAdvancedControlsLoader.html</anchorfile>
      <anchor>a3982098a91072a98e0dbf62ecb588d19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AdvancedControlsOnOff</name>
    <filename>structAdvancedControlsOnOff.html</filename>
    <member kind="enumvalue">
      <name>ONOFF_BLINK</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09dabf0795fb1c1309aa42e6ba075f38ff54</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ONOFF_FADE</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09da14399af55d4af787a6f4ee0fd2e099ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ONOFF_SWITCH</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09da7bf5a07d9ff3bf4673514a110a1553b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>OnOffAnimations</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ONOFF_SWITCH</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09da7bf5a07d9ff3bf4673514a110a1553b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ONOFF_FADE</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09da14399af55d4af787a6f4ee0fd2e099ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ONOFF_BLINK</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ab5e349d4d4c267837af739df2288e09dabf0795fb1c1309aa42e6ba075f38ff54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>switchAnimationTime</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>a7680e3d1d2a7a19a66ac388534414eb6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>switchOffAnimation</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>a7fb0a29efe7803a2ab2edd56aefd043d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>switchOnAnimation</name>
      <anchorfile>structAdvancedControlsOnOff.html</anchorfile>
      <anchor>ace6637d2012fce64fe7460e2b48fc68b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HTTPAsyncRequestHandler::AsyncHttpRequest</name>
    <filename>structHTTPAsyncRequestHandler_1_1AsyncHttpRequest.html</filename>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>structHTTPAsyncRequestHandler_1_1AsyncHttpRequest.html</anchorfile>
      <anchor>a02dcf8daf148eecc8ef6e5234533907a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>long</type>
      <name>receivedTime</name>
      <anchorfile>structHTTPAsyncRequestHandler_1_1AsyncHttpRequest.html</anchorfile>
      <anchor>a8eb4e78ff521c61c0967625e961fb3b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>requestData</name>
      <anchorfile>structHTTPAsyncRequestHandler_1_1AsyncHttpRequest.html</anchorfile>
      <anchor>a17af5fcd268cb43b0492ac3619382c77</anchor>
      <arglist>[400]</arglist>
    </member>
    <member kind="variable">
      <type>AsyncRequestState</type>
      <name>state</name>
      <anchorfile>structHTTPAsyncRequestHandler_1_1AsyncHttpRequest.html</anchorfile>
      <anchor>a946cb172385007db3bb8a1f43481bcfe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>AsyncRequestType</type>
      <name>type</name>
      <anchorfile>structHTTPAsyncRequestHandler_1_1AsyncHttpRequest.html</anchorfile>
      <anchor>abaeeb1c5f558d25d01592dc3b9567677</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>BounceInAnimation</name>
    <filename>classBounceInAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>BounceInAnimation</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a18436464652d39a832de2315cf7e45b1</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount, uint16_t center=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~BounceInAnimation</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>ae76208da05ce01de625dc845ce9f0107</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a3167d6e81d5af3e497e30ba8721d0dcf</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isInProgress</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>ae12a3b8f0ac0b9b637db415992489299</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a29f6ec7642bcb90fb9c00fdbedb69e30</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreColors</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a1e8555d3700c1b976d01a78ba0f559fe</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a59a406d93673e810a9ea979132486477</anchor>
      <arglist>(bool) override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>animationBuffer</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a3abdddec7648f5ae20516cfbbb8555f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>center</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>ac197d2f7046b1ac99242d4166488ce60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a2c5834391f06d0a5620fde309c3053cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>ledsCount</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>ae42cd142fd41bd13d29ec6bfe7c1d3f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>afa56c317b79b0271fe07e10f372e5427</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>radius</name>
      <anchorfile>classBounceInAnimation.html</anchorfile>
      <anchor>a2044f2dbb4105db2c83a831cce97feb0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ComplexSequenceAnimation::Burst</name>
    <filename>structComplexSequenceAnimation_1_1Burst.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>held</name>
      <anchorfile>structComplexSequenceAnimation_1_1Burst.html</anchorfile>
      <anchor>ab32630104a221426cfcc148cfcb30839</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>hold</name>
      <anchorfile>structComplexSequenceAnimation_1_1Burst.html</anchorfile>
      <anchor>affdef7fce370ab139eb764c15adade9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>idx</name>
      <anchorfile>structComplexSequenceAnimation_1_1Burst.html</anchorfile>
      <anchor>aed488267f46668139f48c4ac2148a343</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>step</name>
      <anchorfile>structComplexSequenceAnimation_1_1Burst.html</anchorfile>
      <anchor>a72b6ac5dcd0a01f1a18c6e09b30093c8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ComplexSequenceAnimation</name>
    <filename>classComplexSequenceAnimation.html</filename>
    <base>ILiveAnimation</base>
    <class kind="class">ComplexSequenceAnimation::Burst</class>
    <member kind="function">
      <type></type>
      <name>ComplexSequenceAnimation</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>afb8094f853a2590fa0de91a1ec2efa29</anchor>
      <arglist>(const Config &amp;c=Config())</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ComplexSequenceAnimation</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a1dd9d6dabcda38ee5fbef927d092f89d</anchor>
      <arglist>() override=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a58c2ddd719dc001d83c4c8021d85df68</anchor>
      <arglist>(LedColor *stripPtr, uint16_t count) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>onStart</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a0486c5b30f907e5c6a784243207778f2</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a8914c2ac89a76728375bada20559341b</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="variable">
      <type>Config</type>
      <name>cfg</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>aefb232d6d3018e1325c67c0e8a790328</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="private">
      <type></type>
      <name>State</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>ad557e45f02e59796202a336a896075c4</anchor>
      <arglist></arglist>
      <enumvalue file="classComplexSequenceAnimation.html" anchor="ad557e45f02e59796202a336a896075c4a6d97c716eea277f7262226c6213a7240">SplitFade</enumvalue>
      <enumvalue file="classComplexSequenceAnimation.html" anchor="ad557e45f02e59796202a336a896075c4a8310cebd828de157926a23da69037200">RandomCycle</enumvalue>
      <enumvalue file="classComplexSequenceAnimation.html" anchor="ad557e45f02e59796202a336a896075c4a6160a53ece50ce6d4f5ef4452549835e">Slide</enumvalue>
      <enumvalue file="classComplexSequenceAnimation.html" anchor="ad557e45f02e59796202a336a896075c4a2b31634e3cfef1bfdd7d0d2cdfdc3f9d">Collapse</enumvalue>
    </member>
    <member kind="function" protection="private">
      <type>LedColor</type>
      <name>blendColor</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a3a940fd0d683bcfef2d8a1bf604c6a30</anchor>
      <arglist>(const LedColor &amp;orig, float factor) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>LedColor</type>
      <name>computeAvg</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a9e56fb7d5e73247931c127cfffc82b06</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>processCollapse</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a4e20c6e26e3dda711cc13fd44ccd9062</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>processRandomCycle</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a18019d6324a4169785f8018e276b16a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>processSlide</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>afbba5abf01977b6b86b1d5b82e26060c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>processSplitFade</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a609d2528df69ecdbe8160a5338b37517</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor</type>
      <name>avgColor</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a9977c14a71036bf384c2fee989b9dd8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; Burst &gt;</type>
      <name>bursts</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a3b5506e6e1bb2c5d2696a82a07128c4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>burstsIssued</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a3075a5281f1f28259bb7206440a2d3a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>centerLeft</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>ad682521d0fef09b21c641cded1085b26</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>centerRight</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>ae11027fd868ad3b600e7cd2f16c2b5a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>collapseStep</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a377eee8b0d10bba0f8be193025ca49df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>slideForward</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>af8ba8d9ede4fb1236abf25b29b0e16d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>slidePos</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a70ae75b7b98bb5a375841f727a418579</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>splitStep</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a3eec1386640b885064be821442cffcaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>enum ComplexSequenceAnimation::State</type>
      <name>state</name>
      <anchorfile>classComplexSequenceAnimation.html</anchorfile>
      <anchor>a6e100c68cd7c5304eb5b4c4d1c461f08</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Config</name>
    <filename>structConfig.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>fadeSteps</name>
      <anchorfile>structConfig.html</anchorfile>
      <anchor>aae81b730171d743596b643b222cfd7a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>holdSteps</name>
      <anchorfile>structConfig.html</anchorfile>
      <anchor>a64b0243ea79dc8b270ccec9e25d721ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>maxBursts</name>
      <anchorfile>structConfig.html</anchorfile>
      <anchor>a5470941e0b707f62eac826bc15fb7abb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>slideWidth</name>
      <anchorfile>structConfig.html</anchorfile>
      <anchor>a035f3c17f49626ee80e04e8d6fe69bc0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ConfigData</name>
    <filename>structConfigData.html</filename>
    <member kind="function">
      <type>uint16_t</type>
      <name>getSize</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a585078f011ddd2782411620ca94fa9f7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>serialPrint</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a35946611b2e74b30eae6c6a2858e78cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPanelPassword</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a47e2eaba0fa2d080677c6dac3e82a9b0</anchor>
      <arglist>(String &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPassword</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a6a3f720682dba7c909e7110cd5f59bc0</anchor>
      <arglist>(String &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSSID</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a31af9b9606988854174b8ab2a9bf22c1</anchor>
      <arglist>(String &amp;str)</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isHttpServer</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a1fc06ddc6534bbaebe973438143239d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isRcServer</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>abf40fea3391f7a4c9e119818172f0b4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>networkPassword</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>afac82d6e486b519d5666504b87b4a0f7</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>networkSSID</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a5395b65d16ca51f89bce331c5233788c</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>nodeType</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a75004c1ae3349ef078c5f6401dbb7359</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>panelPassword</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>aa44f7151fc01d2f0a20226eb62ccbc8f</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>reserved</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a1b06f41f941399857fe4d1df49f78ea2</anchor>
      <arglist>[150]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>safeShutdownFlag</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a05e1c0cbebe6c4b1683014bb9b46cf56</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ConfigProvider</name>
    <filename>classConfigProvider.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a54e4105ac91fd6224bd5630ac2882ca5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>acdb6ebd4a5283bf88148e8628b42693c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>eraseDatablockMemory</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a8a405128cb31660284c694f3ec042b30</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flushNvmData</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a9f40c1de6461d976d853a58e599dcd24</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>getConfigJson</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a05cd6f1fc601bc02a34f8348a6a32f9b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>getDatablock</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a320aea31f39f9165e27f0852025aa342</anchor>
      <arglist>(PersistentDatablockID blockID, uint8_t *buffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>af525c8dd3b77a540741010e08eaa5515</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>loadConfigFromFile</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a789eb408af6aa870af347a9282ff3279</anchor>
      <arglist>(JsonDocument &amp;doc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>massErase</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a7335967378e9c33c0149db2f294b53cd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>setConfigViaString</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a87032790e5c24e27cfdf75a04cb9b312</anchor>
      <arglist>(String &amp;configString)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>setDatablock</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a0c9f990abc3ed4af704801d5483656d1</anchor>
      <arglist>(PersistentDatablockID blockID, uint8_t *data)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>readRamMirrorFromNvm</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a62488e7f4af700b70acad9024a81a036</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>saveRamMirrorToNvm</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>ac6ce2e0e3e7deafe298dbfa56240653e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateNodeConfigurationSignal</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>af43ad9482b604b9da54aeb8419b7143f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ConfigData</type>
      <name>configRamMirror</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>ab5603d4bff123721b25df46ace2b323a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static PersistentDataBlock</type>
      <name>dataBlocksRamMirror</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>abb6a791f3fdb81e1d8e76d1a2d6fc6af</anchor>
      <arglist>[PersistentDatablockID::e_NUMBER_OF_PERSISTENT_BLOCKS]</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>nvmDataAvailable</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a84c8e96a0736817e3523addd7fea51a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>totalNvmSize</name>
      <anchorfile>classConfigProvider.html</anchorfile>
      <anchor>a91eb38ea1dc7d868375875ddb37972ab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ConfigSlotsDataType</name>
    <filename>structConfigSlotsDataType.html</filename>
    <member kind="variable">
      <type>std::array&lt; DeviceConfigSlotType, 6 &gt;</type>
      <name>slots</name>
      <anchorfile>structConfigSlotsDataType.html</anchorfile>
      <anchor>ae83d90119ac5194919c0d751b90808e3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CyclicProfiler</name>
    <filename>classCyclicProfiler.html</filename>
    <class kind="struct">CyclicProfiler::Stat</class>
    <member kind="typedef">
      <type>void(*)()</type>
      <name>FuncPtr</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>ad2995a44a6d562329466756cb44f63cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>call</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>ac742be4f94acc60468e39b1007f14545</anchor>
      <arglist>(const char *name, FuncPtr func)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reportEvery</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>a3be1ede650c48456939abd8fb5b5e968</anchor>
      <arglist>(unsigned long ms)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setLastIterations</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>a40b48669247c22fa9697b5a87de50f3c</anchor>
      <arglist>(int it)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>currentIterations</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>ad8e10db68b64c7b576557948bc493aee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>nextReport</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>a2727dcf53c1f99a070565b1bab193c86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; String, Stat &gt;</type>
      <name>stats</name>
      <anchorfile>classCyclicProfiler.html</anchorfile>
      <anchor>aa2ddc522de34d036a6923d7dde8c66cd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DataContainer</name>
    <filename>classDataContainer.html</filename>
    <member kind="function">
      <type></type>
      <name>DataContainer</name>
      <anchorfile>classDataContainer.html</anchorfile>
      <anchor>a76e1e60d717a64b7f742699f67c76276</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::any</type>
      <name>getSignalValue</name>
      <anchorfile>classDataContainer.html</anchorfile>
      <anchor>ac2e999ee664864572c53a208f5412141</anchor>
      <arglist>(Signal sigName)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setSignalValue</name>
      <anchorfile>classDataContainer.html</anchorfile>
      <anchor>a177aaa08f6871c451af33354a5e68bf9</anchor>
      <arglist>(Signal sigName, Type newValue)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>subscribe</name>
      <anchorfile>classDataContainer.html</anchorfile>
      <anchor>aa3ab52df999443fb24c9824c2a08d1fb</anchor>
      <arglist>(Signal sigName, std::function&lt; void(std::any)&gt; callback)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::array&lt; std::any, 48 &gt;</type>
      <name>dataTable</name>
      <anchorfile>classDataContainer.html</anchorfile>
      <anchor>aad48e096d1285625edb3ba37f196eec1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::array&lt; std::vector&lt; std::function&lt; void(std::any)&gt; &gt;, 48 &gt;</type>
      <name>subscribers</name>
      <anchorfile>classDataContainer.html</anchorfile>
      <anchor>a9adcefd42058a09def0e8ff12a68290a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Device</name>
    <filename>classDevice.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a8d0638fea9ff80bf80627ed684f4e051</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a245aa604bbe9a24171eeb8a4dd34e4cd</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a76583be58ff9ac8d40fdf32e75ddfed6</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a220eb86524a9dec1ec6624aa48cb1754</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a8956c8cecb40e2d2342e75c6803590e3</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a20d10b3501aa4df9c94a4b0b3c7f9caf</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a57750faec69dece984d782a4ce0b47e9</anchor>
      <arglist>(DeviceServicesType serviceType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a5399e322b0c8d7cbb4cd3d033f99253d</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a04028f2868c08bfbbdc35f46d66f99a7</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>a1161967d79f1d9b09da0e92793ade5d2</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)=0</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>DeviceIdentifier</name>
      <anchorfile>classDevice.html</anchorfile>
      <anchor>af6702dc3965b043931c772174260d599</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DeviceConfigManipulationAPI</name>
    <filename>structDeviceConfigManipulationAPI.html</filename>
    <member kind="variable">
      <type>std::function&lt; String()&gt;</type>
      <name>getDeviceCfgJson</name>
      <anchorfile>structDeviceConfigManipulationAPI.html</anchorfile>
      <anchor>a15c762ab07d8d27608877e6db356ead6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; bool(JsonDocument &amp;)&gt;</type>
      <name>loadConfigFromFile</name>
      <anchorfile>structDeviceConfigManipulationAPI.html</anchorfile>
      <anchor>ac645c5b109f610c3e762e353d908610d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; bool(String &amp;)&gt;</type>
      <name>setDeviceCfgViaJson</name>
      <anchorfile>structDeviceConfigManipulationAPI.html</anchorfile>
      <anchor>a10b734f47aef01503928fcd4521ffe9b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DeviceConfigSlotType</name>
    <filename>structDeviceConfigSlotType.html</filename>
    <member kind="function">
      <type></type>
      <name>DeviceConfigSlotType</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a495e8847373c7bf824f38945c96a19f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>af3a979c72ca6f6561d1a811e6ccde298</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a940fb1f5524311724936934ce596ff51</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint8_t</type>
      <name>getSize</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a5c59d297b5806d116463f092837ddf76</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>customBytes</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>aa7f23be16928362f055b9a111d8f00d8</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a2d9e6eb7b6fb30fb2b55da58d972374d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>deviceName</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a3a00a18c602685e40b0bea3bbf853377</anchor>
      <arglist>[25]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>deviceType</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a43aa0a984da636e7f83654b23327f094</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isActive</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a9ab9d57f8fc26ba9af9e97f247cf1bdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>pinNumber</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a33ba3b41dbaf3e76b05cebeb4dfb01d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>structDeviceConfigSlotType.html</anchorfile>
      <anchor>a3506cd8bddc3692aebd9cfd6db72159e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DeviceControlFunctionSet</name>
    <filename>structDeviceControlFunctionSet.html</filename>
    <member kind="variable">
      <type>std::function&lt; bool(uint8_t, uint8_t)&gt;</type>
      <name>changeBrightness</name>
      <anchorfile>structDeviceControlFunctionSet.html</anchorfile>
      <anchor>ac8d4f09bcdc1af38ad0e2c54b31a1046</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; bool(uint8_t, bool)&gt;</type>
      <name>setDeviceState</name>
      <anchorfile>structDeviceControlFunctionSet.html</anchorfile>
      <anchor>acfffc6b0f18fc876c7fac6f9f54f67ca</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DeviceDescription</name>
    <filename>structDeviceDescription.html</filename>
    <member kind="function">
      <type></type>
      <name>DeviceDescription</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>ab022d3c9727712d5a9ce76a5fe7ac7a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>fromByteArray</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>af3882fe5ad80a6e4b8afa54cc68b83b1</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>getSize</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>a7b2442d8ddc6bed3fc932e8f628e6cc0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>a48f0ececf9e852f4758b26531f24c928</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toByteArray</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>aa030df4f29ad988e4b9f1c223b844561</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>customBytes</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>aceb2d4e5b37d5a45119955df825d979f</anchor>
      <arglist>[50]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>a5af679febb655927e1883b2780492357</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>deviceName</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>a88c70cdccae45cbcf5eac987369dfb88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>deviceType</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>abc68fb4afbc8cdc1997fd6e986cb5960</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isEnabled</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>ae4d556fd1e1ca71261b30d3c67f939ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>macAddress</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>a5f3d45ea20c021459049cc68147f2833</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>structDeviceDescription.html</anchorfile>
      <anchor>aa0b4c23c1e092faa03854f9971bf04be</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DeviceDescriptionPacker</name>
    <filename>classDeviceDescriptionPacker.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>addChunkToCollection</name>
      <anchorfile>classDeviceDescriptionPacker.html</anchorfile>
      <anchor>a783e651311bbe012e161c5193de4f845</anchor>
      <arglist>(std::vector&lt; DeviceDescription &gt; &amp;collection, const std::vector&lt; DeviceDescription &gt; &amp;chunk)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::vector&lt; DeviceDescription &gt;</type>
      <name>getChunk</name>
      <anchorfile>classDeviceDescriptionPacker.html</anchorfile>
      <anchor>abb299d0c8697df8c42aafc058be438ff</anchor>
      <arglist>(const std::vector&lt; DeviceDescription &gt; &amp;devices, uint8_t chunkIndex)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint8_t</type>
      <name>getRequiredChunkCount</name>
      <anchorfile>classDeviceDescriptionPacker.html</anchorfile>
      <anchor>a96ccd54d5c421e2ca01f52ff6bffb9a2</anchor>
      <arglist>(uint16_t deviceCount)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::vector&lt; std::vector&lt; DeviceDescription &gt; &gt;</type>
      <name>packDeviceDescriptions</name>
      <anchorfile>classDeviceDescriptionPacker.html</anchorfile>
      <anchor>acef4ca0f7beb8a1864c2a0bd5506830b</anchor>
      <arglist>(const std::vector&lt; DeviceDescription &gt; &amp;devices)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::vector&lt; DeviceDescription &gt;</type>
      <name>unpackDeviceDescriptions</name>
      <anchorfile>classDeviceDescriptionPacker.html</anchorfile>
      <anchor>a11a24c455dc5280d90c2a60c8dd5fb91</anchor>
      <arglist>(const std::vector&lt; std::vector&lt; DeviceDescription &gt; &gt; &amp;chunks)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static constexpr uint8_t</type>
      <name>MAX_DEVICES_PER_CHUNK</name>
      <anchorfile>classDeviceDescriptionPacker.html</anchorfile>
      <anchor>aea02b328c716a3956e67e25caa37a41e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DeviceManager</name>
    <filename>classDeviceManager.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>abede853a2004bc9a48d3b2a68d1b8cb1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ada6b112849143ca893cabe765d410718</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flushNvmData</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a8fab75625581e3633b23f48a63d2fc5d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>getLocalSetupJson</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a3079d22dd0e36c7604c23690e5f39492</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static RtcTime</type>
      <name>getRtcTimeWrapper</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a81fdbe1c8f8005b5927b2bbb89259108</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a11625c94f5c2af343124964eb2653354</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>loadConfigFromFile</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a47dd720f64ad71b8c4f31ca0cf273cf6</anchor>
      <arglist>(JsonDocument &amp;doc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>persistentDataChanged</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>aa997c044c22d9c401484ccb19ba059fd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>aef4ff2ca719503a9b9ec1ceecc9db93c</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ac613e22dfc7a9aed8369657f90ef20fd</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>aa9167185e6e786de34efdf911c7c2c9a</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a7a39ecb6359b0b9d62eba5d3c3c44a3f</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>setLocalSetupViaJson</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ac33144dec4a580a3a3350c7258345ec8</anchor>
      <arglist>(String &amp;json)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>extractDeviceInstanceBasedOnNvmData</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>acc7c90ea6c943f89203ffdda9520abb1</anchor>
      <arglist>(DeviceConfigSlotType &amp;nvmData, uint8_t configSlotID)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateDeviceDescriptionSignal</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>aadb1e0ca4205944ae186c02d4844fb89</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; Device * &gt;</type>
      <name>devices</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ab165f749dcd092562343731090213417</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; DistanceSensor &gt;</type>
      <name>distanceSensorsDevices</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a1ff5a8e9a87e5b1fad671223340fdf4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ExtendedDataAllocator</type>
      <name>extDataAllocator</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ad88a4a55f00c2db15e2bd28554288e75</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; HwButton &gt;</type>
      <name>hardwareButtons</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a842253e242d5f742fbf5d6228b79eec4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; LedWS1228bDeviceType &gt;</type>
      <name>ledws2812bDevices</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a62147cb951dc34178e50f5c29dd28048</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ConfigSlotsDataType</type>
      <name>pinConfigSlotsRamMirror</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ac52a3e3a2644b14c12a20e2992c99be1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; SegLedWS1228bDeviceType &gt;</type>
      <name>segmentedWs2812bDevices</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>aa4b27eccaedacadd69bb43381e8e8428</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; TempSensorDHT11DeviceType &gt;</type>
      <name>tempSensorsDevices</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>a4722b70566eaa646419df84692c5f910</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; OnOffDevice &gt;</type>
      <name>vecOnOffDevices</name>
      <anchorfile>classDeviceManager.html</anchorfile>
      <anchor>ac296d1132efabc5a4f5cef3206fe4056</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DeviceProvider</name>
    <filename>classDeviceProvider.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>af175cb3fadb8c880967088603bfbc8a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a243c5fafdfcee3570c677528f7d22fc4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deviceReset</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>ad0793ea672a6eb0b600574fff6be631a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a4cc2bc64f65b733723f87a16007cfc1e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>printIdMap</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>ad3fdd3de3d94be6957186f9dba4e85e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>receiveRequest</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a5ab42bd6f62090ab5cf05eb82cf9e720</anchor>
      <arglist>(RcRequest &amp;request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a64eef14ed1a833ba761375601f5395fd</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>ad00bd09866f1e3e0606a37e021616941</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a2bc8b90dcca7c9ae2281bcdccd77bcb1</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a472e06a10235f147583c5aeb986c13fe</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>updateDeviceDescriptionSignal_onChange</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a9975c2b39996aaf7a5885183e00eb86c</anchor>
      <arglist>(std::any signal)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>addDeviceDescriptionToResponsePayload</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a4a259fd2c445ba3deb9fe2359d5b0912</anchor>
      <arglist>(RcResponse &amp;response, uint8_t deviceId)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static DeviceTranslationDetails</type>
      <name>getOriginalIdFromUnique</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a235561234f0bd4a6c444272d1d15db7f</anchor>
      <arglist>(uint8_t uniqueId)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>handelService3Request</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>ac79c9f6573529ad37d3126b8dfe18016</anchor>
      <arglist>(RcRequest &amp;request, RcResponse &amp;response, ServiceParameters_set3 &amp;param, uint16_t payloadSize)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>initLocalDevicesSetup</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a7def09face3582e3da92c0a1e9606445</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>initRemoteDevicesSetup</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a67960ea4d96ca5b342e5d047ee27e775</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateDeviceDescriptionSignal</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>adf4c881c878f88b43f06dddb7f9b0937</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>isRCServer</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a0642f9a9df069492b8233e9eeb474e5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>requestedRoomState</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>ad58877ce6d3d76a295d11d881be23cd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::function&lt; bool(RcResponseLong &amp;)&gt;</type>
      <name>requestResponseLong</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>ac7f3c3b29df6971008c46258867a8b22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; uint8_t &gt;</type>
      <name>roomStateChangeDeviceIdQueue</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a039ab8bd2ecfd64215dbda06db8b40c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::function&lt; bool(RcResponse &amp;)&gt;</type>
      <name>sendResponse</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>a121bf22907ae36dbeb1dc06dfc942592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint8_t, DeviceTranslationDetails &gt;</type>
      <name>uniqueDeviceIdToNormalDeviceIdMap</name>
      <anchorfile>classDeviceProvider.html</anchorfile>
      <anchor>aad63f9b3c3a466b4dc477561ffc51519</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DeviceServicesAPI</name>
    <filename>structDeviceServicesAPI.html</filename>
    <member kind="variable">
      <type>std::function&lt; ServiceRequestErrorCode(uint8_t, DeviceServicesType)&gt;</type>
      <name>serviceCall_NoParams</name>
      <anchorfile>structDeviceServicesAPI.html</anchorfile>
      <anchor>a1e33666b42c6f444faf83434d9ba9e6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; ServiceRequestErrorCode(uint8_t, DeviceServicesType, ServiceParameters_set1 param)&gt;</type>
      <name>serviceCall_set1</name>
      <anchorfile>structDeviceServicesAPI.html</anchorfile>
      <anchor>aa172a7b681713f22f6723d50b271f54d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; ServiceRequestErrorCode(uint8_t, DeviceServicesType, ServiceParameters_set2 param)&gt;</type>
      <name>serviceCall_set2</name>
      <anchorfile>structDeviceServicesAPI.html</anchorfile>
      <anchor>ace27f1171a1ef5f5ccb1f1956271616a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; ServiceRequestErrorCode(uint8_t, DeviceServicesType, ServiceParameters_set3 param)&gt;</type>
      <name>serviceCall_set3</name>
      <anchorfile>structDeviceServicesAPI.html</anchorfile>
      <anchor>a8f87ff14630b7972860f6d843207b99f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DeviceTranslationDetails</name>
    <filename>structDeviceTranslationDetails.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>isLocal</name>
      <anchorfile>structDeviceTranslationDetails.html</anchorfile>
      <anchor>a67acd79a5d73da858ac965a4e4e70926</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>originalID</name>
      <anchorfile>structDeviceTranslationDetails.html</anchorfile>
      <anchor>af708769ed85c44e8cbc05635e94f344f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DigitalEventReceiver</name>
    <filename>classDigitalEventReceiver.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>ac2be1f1c55675c6c77822b2db13c414a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a85a71a9eda53fbfd989b303adfd306a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>fireEvent</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a003827f217ef9841d01bb5287c2db3bd</anchor>
      <arglist>(uint64_t eventId)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a694d8e5cc1800f4f4a791e8462d6b63a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>receiveUDP</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a264fc90498966b4be2a9ad71c9ab6a85</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>updateDigitalEventMappingViaJson</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a10d752a88994b0167a6add623252d896</anchor>
      <arglist>(String &amp;json)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>deviceAction</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a3da0f8cbfc41e27a0e1e324699547976</anchor>
      <arglist>(DigitalEvent::Event &amp;action)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>executeAction</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>abe912692da00b12b23df3f3fe2ade49c</anchor>
      <arglist>(DigitalEvent::Event &amp;action)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processEvents</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a9b8189f33b9d6b0ed50f7b0a3a4cab63</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>roomAction</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a9dca3356b18a70932b82ca152f7d9259</anchor>
      <arglist>(DigitalEvent::Event &amp;action)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; std::pair&lt; uint64_t, DigitalEvent::Event &gt; &gt;</type>
      <name>digitalEventsMapping</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>abc702793dd9644b5fc7a8efcdc90229e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; uint64_t &gt;</type>
      <name>eventsQueue</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a6ed82760865642ce91b33f01a48ba956</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long</type>
      <name>lastEventOccurrenceTime</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>ad8575263c25023bdf736bd36a2573d71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>lastReceivedTransmissionId</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>add6211cccfb7c79cfcc6b3a777c5aa93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; ServiceCallData &gt;</type>
      <name>pendingServiceCalls</name>
      <anchorfile>classDigitalEventReceiver.html</anchorfile>
      <anchor>a749d669f21708c8036d0aebae8f239fc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DigitalEventTransmitter</name>
    <filename>classDigitalEventTransmitter.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a805756d2c1b03f1c2ffd9495d7f5782f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>ae0a59a59f7fd373ac835357327ec200d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>fireEvent</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a18427fd331fc8c5768946b0bcbeffff0</anchor>
      <arglist>(uint64_t eventId)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a1ac1b3e136cab8fc4279f9bd8364878c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>receiveUdp</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a8e772376d2633ba26867adb34b44d330</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint64_t</type>
      <name>lastTransmittedId</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a8726955031b7fdd511b6c93f82b5bea4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long long</type>
      <name>lastTransmittedTime</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a8363c578e94de0fa5d301841f6752108</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>transmissionIdentifier</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>afc09560493f6cc14852634c01e80f4a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>wasRequestRepeated</name>
      <anchorfile>classDigitalEventTransmitter.html</anchorfile>
      <anchor>a854ce9876b74e7c2f33ae9a7382f2b0b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Display</name>
    <filename>classDisplay.html</filename>
    <member kind="function">
      <type></type>
      <name>Display</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>a60fec3eb6f87e7c1e89b9df7d7c75bc0</anchor>
      <arglist>(DisplayInfo info)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>getDisplayDataHash</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>ad2a323f8bb04d1486e1df7d92af50ecb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>getLastDataUpdateTime</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>ad97fe9fc02db53cdf888076048a02d38</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>a97f44f15f28597d1d1c0b6852c6973b9</anchor>
      <arglist>(const Display &amp;other) const</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>displayDataHash</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>a78cae962b9dae6c837a7cff280416368</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>DisplayInfo</type>
      <name>info</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>a8fef33de2438e25e7708af57f0dfc02d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>lastDataUpdateTime</name>
      <anchorfile>classDisplay.html</anchorfile>
      <anchor>ab3429013ecf39fddf1579aad5e28af24</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DisplayInfo</name>
    <filename>structDisplayInfo.html</filename>
    <member kind="variable">
      <type>MessageUDP::IPAddr</type>
      <name>ip</name>
      <anchorfile>structDisplayInfo.html</anchorfile>
      <anchor>a48520bb25b2d2bfe001330ff92afeae7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>macAddress</name>
      <anchorfile>structDisplayInfo.html</anchorfile>
      <anchor>a2d70b96ca88f40ed4a1541249964ef04</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>resHeight</name>
      <anchorfile>structDisplayInfo.html</anchorfile>
      <anchor>a80f8b0df18eaf2b7862fdcd6c9e935d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>resWidth</name>
      <anchorfile>structDisplayInfo.html</anchorfile>
      <anchor>aae2175fdb30b35f7247f49a6a2f77e72</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DisplayServer</name>
    <filename>classDisplayServer.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>aa955e461798d6db9c0e9f211129f85c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a4fb5a4802988041928da3537523e8b56</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a30b96d312c914b1878f355a3e00f8e5d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>receiveUDP</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>af6cfbd876679601e8c3d2a953110a811</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>discoverDisplays</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a0be1666223383fc9beec01a87aaa016f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleNewDisplay</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a0418a11cc004c92d9366c57bb9f714b7</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processReceptionBuffer</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a33ca2593793362e89ded2ea278164a46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; Display &gt;</type>
      <name>knownDisplays</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a4406f603b67a134cedef31a8dee7e739</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>lastDiscoveryTime</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a49a94621eb71dee4c1db491618230d40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; MessageUDP &gt;</type>
      <name>receptionUdpQueue</name>
      <anchorfile>classDisplayServer.html</anchorfile>
      <anchor>a11c451c2f2f17eefc84d4400d4d47159</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DistanceSensor</name>
    <filename>classDistanceSensor.html</filename>
    <base>Device</base>
    <member kind="function">
      <type></type>
      <name>DistanceSensor</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a96526b9f3be4b69e2b8e849c11a983d9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DistanceSensor</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>ae0c714ad650ffffe2320575576b5cf43</anchor>
      <arglist>(DeviceConfigSlotType nvmData)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a658ce0c15811ff87f83fd10d8280df93</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a0bd79ad183a656cfff33e47067a668bb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>ab09424fba5f5e9e1280297e777e54fa7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a04e2ed765bc075ea1a500bdbf0ef2b8f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>af6675707a0770f8443b607a1149c9cb5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a0d5e4b8cf425987e78aef4e02b4afaf0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a333d1d1d12318994e860ead8623eeb84</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a115d6981545287b3acab72bc466a9a90</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a533ab98f72d55b413cc3b316e398a456</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>aebe7a57d4f1613f9610f3d296007982e</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>aff5c2005c894a0aaf1b107fb01a9f6a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>String</type>
      <name>deviceName</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>ac81ec54c67322d344deaa7ffd63f542f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pinNumber</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a827d5879360be4e67a840059d27ad1d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>classDistanceSensor.html</anchorfile>
      <anchor>a7a82df494580dc43f7368740503758cd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ExtendedData</name>
    <filename>structExtendedData.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>identifier</name>
      <anchorfile>structExtendedData.html</anchorfile>
      <anchor>abbd74781f0f8a719796ec527af006f53</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>length</name>
      <anchorfile>structExtendedData.html</anchorfile>
      <anchor>a5c8569190bbf0eec765c8ccde5a3b577</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>memoryPtr</name>
      <anchorfile>structExtendedData.html</anchorfile>
      <anchor>aba1a3f48814710c194173b5484db014d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ExtendedDataAllocator</name>
    <filename>classExtendedDataAllocator.html</filename>
    <member kind="function">
      <type></type>
      <name>ExtendedDataAllocator</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>a02921b31699e357214f00268c96bd0ae</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>ExtendedData</type>
      <name>createNewExtendedData</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>aa3e14592381e3e4ede0329141aaad3c3</anchor>
      <arglist>(uint16_t length)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deleteExtendedData</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>a57ddbcf4ec94a4fd2a505fe652feb43b</anchor>
      <arglist>(uint8_t identifier)</arglist>
    </member>
    <member kind="function">
      <type>ExtendedData</type>
      <name>getExtendedDataById</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>aba785ab92ec3c9ea00d883972be047c3</anchor>
      <arglist>(uint8_t id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printExtendedDataContainer</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>a5eae9c758f231f9df65cb4c1b9350ee9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; ExtendedData &gt;</type>
      <name>extendedDataContainer</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>a9e28dc5d54578ccb61456c97f4f9a63b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>extendedDataId</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>a0e1611d7b7754dc9150979a6900f496c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; uint8_t * &gt;</type>
      <name>memoryBlocks</name>
      <anchorfile>classExtendedDataAllocator.html</anchorfile>
      <anchor>a4eea864898f6f05ab0a87b6d90d82132</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ExtendedMemoryCtrlAPI</name>
    <filename>structExtendedMemoryCtrlAPI.html</filename>
    <member kind="variable">
      <type>std::function&lt; uint16_t(void)&gt;</type>
      <name>getCurrentMemoryUsage</name>
      <anchorfile>structExtendedMemoryCtrlAPI.html</anchorfile>
      <anchor>ab91c6dcd06e76c1a4de6bacc4c95363e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; uint8_t *(uint8_t, uint16_t *)&gt;</type>
      <name>getExtMemoryPtrByDeviceId</name>
      <anchorfile>structExtendedMemoryCtrlAPI.html</anchorfile>
      <anchor>aeb7076e5362342257e15782f5d95761e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; void(uint8_t)&gt;</type>
      <name>releaseExtendedMemorySpace</name>
      <anchorfile>structExtendedMemoryCtrlAPI.html</anchorfile>
      <anchor>a5bbc9718ccc9fd290eaacb5ad76e6437</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; bool(uint8_t, uint16_t)&gt;</type>
      <name>requestNewExtendedMemorySpace</name>
      <anchorfile>structExtendedMemoryCtrlAPI.html</anchorfile>
      <anchor>a65db049ae48dc70f206e1c89c8295563</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ExtendedMemoryManager</name>
    <filename>classExtendedMemoryManager.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a70105d1322ddfcfa46df85d67b23d54a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a0ef56432f37b016862175043290205e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flushNvmData</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a5ad3a023d9b7d77499ba4b7f4253029a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint16_t</type>
      <name>getCurrentMemoryUsage</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>aac99a3bf956fe7817c39bb7dc514ce13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint8_t *</type>
      <name>getExtMemoryPtrByDeviceId</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>ab4dc1cb4b7a68259b29983bc5ceaf43c</anchor>
      <arglist>(uint8_t deviceId, uint16_t *extMemoryLengthPtr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>ad5d13a0f1be388ed35b87a3547dfbcfd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>releaseExtendedMemorySpace</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>afcb444fbec673cc2edb4b12109caf732</anchor>
      <arglist>(uint8_t deviceId)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>requestNewExtendedMemorySpace</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>ab94743fdbe8cbf9c8f68461798b51736</anchor>
      <arglist>(uint8_t deviceId, uint16_t spaceSize)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>restoreExtMemoryFromNvm</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a419119bc998961e7db3bd28b39b9478f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateCurrentExtMemoryUsage</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a7753cb2efe48d4535339afc6f8cf8815</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; ExtMemoryData &gt;</type>
      <name>extMemoryContainer</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a6f8879e45a97e2a8f43db3389f6b191f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>extMemoryInUse</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>a93dc1e595b765c5209068ba00db1d538</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ExtMemoryMetadataType</type>
      <name>extMemoryMetadata</name>
      <anchorfile>classExtendedMemoryManager.html</anchorfile>
      <anchor>abad9d8990a0658bece7abba03fe0abb1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ExternalNodeMapping</name>
    <filename>structExternalNodeMapping.html</filename>
  </compound>
  <compound kind="struct">
    <name>ExtMemoryData</name>
    <filename>structExtMemoryData.html</filename>
    <member kind="function">
      <type>void</type>
      <name>printExtMem</name>
      <anchorfile>structExtMemoryData.html</anchorfile>
      <anchor>ad5ea518767d4f8f1327727021c6b14ec</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>dataPtr</name>
      <anchorfile>structExtMemoryData.html</anchorfile>
      <anchor>a489ce682689d03cfb2e2577a645f1af4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>length</name>
      <anchorfile>structExtMemoryData.html</anchorfile>
      <anchor>aeb060d85bfd140b09e687257dc4c256a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>ownerDeviceId</name>
      <anchorfile>structExtMemoryData.html</anchorfile>
      <anchor>a7ad15b23e3e5052898c958dc0b4fb580</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ExtMemoryMetadataType</name>
    <filename>structExtMemoryMetadataType.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>memoryPerDeviceSlotNeeded</name>
      <anchorfile>structExtMemoryMetadataType.html</anchorfile>
      <anchor>a24288a15c53e94a933a0fa4ac28e578b</anchor>
      <arglist>[10]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>reserved</name>
      <anchorfile>structExtMemoryMetadataType.html</anchorfile>
      <anchor>a06557bc0e96a211e65647c32ad0fe5a6</anchor>
      <arglist>[50 -(10 *sizeof(uint16_t))]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FadeInAnimation</name>
    <filename>classFadeInAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>FadeInAnimation</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>ad2e40c3e2be22e744cc157bac52367ad</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>FadeInAnimation</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>afd2174faeab38d7a513a9ec6aae882a7</anchor>
      <arglist>(LedColor *colorsPtr, uint16_t ledsCount)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>add33a0436ad396d1350dd0bac60ea2a3</anchor>
      <arglist>(LedColor *colorsPtr, uint16_t ledsCount)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isInProgress</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aeb4e88eac17b10637ab82eeb4bed9392</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>process</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>adfa75b6bd55357f2fa47fde66f2e037f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>restoreColors</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>a8d495311badd3d4526b0a16af8639b34</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>start</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>a78a4709483d1caf80c30d19bfd245f5a</anchor>
      <arglist>(bool startFromZero)</arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Blue</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6fa1b68161c0f941df080c890dfa5f96644</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="private">
      <type></type>
      <name>FadeInColor</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Red</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6fad0ac94108b51c1f3074f32b903e40c6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Green</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6fa2f5285f9c7ffc4bc66444685d522744d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Blue</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6fa1b68161c0f941df080c890dfa5f96644</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Green</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6fa2f5285f9c7ffc4bc66444685d522744d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Red</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>aa5e4f9f133637eaffe2abd1ab1177f6fad0ac94108b51c1f3074f32b903e40c6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>processColor</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>a5b7977f09ddef312dd882e34f47b2122</anchor>
      <arglist>(FadeInColor color, uint16_t diodeIndex)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>processDiode</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>abd19803e541a595b60c65bbb7470778f</anchor>
      <arglist>(uint16_t diodeIdx)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classFadeInAnimation.html</anchorfile>
      <anchor>a4b6961a05fe0562007079af22b9cb81c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FadeOutAnimation</name>
    <filename>classFadeOutAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>FadeOutAnimation</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a9fcc15d72282e17cdda087a4102d5228</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>FadeOutAnimation</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a914eacb591f4f73f8b9cc160d2ae15a5</anchor>
      <arglist>(LedColor *colorsPtr, uint16_t ledsCount)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a782abdb77158fd37703924e776909188</anchor>
      <arglist>(LedColor *colorsPtr, uint16_t ledsCount)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isInProgress</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a70f97fadee60d818f8c98f427885cef5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>process</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>aca3672f790082e4d1043cbff2218a3eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>restoreColors</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a994d2d2c5cd988df2377da1ed958c6de</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>start</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a71ff5825f28b31d259deed6f6d0d77f4</anchor>
      <arglist>(bool startFromZero=false)</arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Blue</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1a2e7615055011e05d0321fdb2d42edad5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="private">
      <type></type>
      <name>FadeOutColor</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Red</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1acf1f4df0d0b47eb2b07649e369613bcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Green</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1a8bdf32c2b43f80d6bb464e3594445971</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Blue</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1a2e7615055011e05d0321fdb2d42edad5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Green</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1a8bdf32c2b43f80d6bb464e3594445971</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>Red</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>a2e2c5caaa6375cf2efed30cb888086b1acf1f4df0d0b47eb2b07649e369613bcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>processColor</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>abdac8d766628b170f713694b1c6dff9e</anchor>
      <arglist>(FadeOutColor color, uint16_t diodeIndex)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>processDiode</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>ab0c51b76f6cb4d91a893c0e1cc93b37f</anchor>
      <arglist>(uint16_t diodeIdx)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classFadeOutAnimation.html</anchorfile>
      <anchor>ac100a0e5dd028576955954e7657cfc31</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HomeLightHttpServer</name>
    <filename>classHomeLightHttpServer.html</filename>
    <class kind="struct">HomeLightHttpServer::HttpServerNvmMetadata</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_asyncGetNotifications</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a43e71163e4b8407c9b60770c9ba8ebb0</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_asyncGetPageContent</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a2687e6efbac15ad6ff2ca81e0a4f8f92</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_asyncTest</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a8cad4e9527a325dff606085d28483fa8</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_configPage</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>aaa557a3c66e1c79996f35895e5451cc7</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_devicesSetup</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a958b5ae2caea952d27d48aab0387f297</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_digitalEvents</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a04f21910b13ab27230d9f214463950f8</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_mainPage</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a7c8995a456caa9756cfffef6fc5f26cc</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_massErase</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a27156e4dc21827ac4a99098fd1a6000c</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_networkInspecion</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a81377d8a1b9bf3ec313aa69538fb94ef</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_resetDevice</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a85840ad67d4aee679bc99b3b6fff8c7c</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_roomAssignment</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>aa522b7c77f3ec54700fa235def6a5f03</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>constantHandler_systemDetails</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a0558d0112a95f59e40cdcb56fe9947a4</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a6dd3c05caeada744c8f355a3f77f9e6b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a10b069a17048a424149794f34fd5eced</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flushNvmData</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a082453cd9fb37f237c9db046b01f5bc3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>generateConfigSlotUi</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a8cc7611d9362d5b8aaa6423aae5fc83a</anchor>
      <arglist>(uint8_t slotNumber, DeviceConfigSlotType &amp;slot, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>getRoomsCfgJson</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a036da19b66cc5be94ba2977f8916999b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a512dac2ee7e22004a8c9398dc3d34860</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>loadConfigFromFile</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a8ef6a3a1a6dae037abbf3df290807878</anchor>
      <arglist>(JsonDocument &amp;doc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>onDeviceDescriptionChange</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a917d09b29daf6225931aa052cb528f12</anchor>
      <arglist>(std::any newDescriptionVector)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_deviceBrightnessChange</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a63b34e820593ce48e1a742e107b0e246</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_deviceSwitch</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a4381e9fb45f0086d61ae353aa3ccee7d</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_downloadDeviceConfiguration</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>aef8de1467a5239298d0f3dd2944d92c4</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_getExtendedControls</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a1e4448829fab277c177373d443e45ba7</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_getHash</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>aace5c3b4986049c6db3a8831383a9eb9</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_ledColor</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a804b503292c41ba8f0e327ffd1307093</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_ledsLiveSwitch</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a09caadbf0ac906b2a02b2b76694d0c5c</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_ledStripColor</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a230bb4ee692ce63f94a3553b9019bcb0</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_loadDeviceConfiguration</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>af7367ce1952dd6cba9ed782163984d5f</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_newConfigApply</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a1da6570712e6e7b5c90710ababbf0263</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_newDigEvntTab</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a81a2a4dca5d988d33fd2d3b96a3e9ede</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_newSetupJson</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>adf53c8be71b8f7db767b7de374322ac2</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_passwordApply</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a74090026b6f02457d4637a9510577c40</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_roomNameMappingApply</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>acbdecd8d0f110685d64329a1311b2957</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_roomStateChange</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a79fb135a72b2bda5542789126b223944</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_roomToggle</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a13c7460bc379de3fb55d691734072b49</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_segmentStateSwitch</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a73ac27234fd161f9b585199fb348be7b</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_setStripColor</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ae8f2616506042553cfb5e32402d52092</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_stripLoadFromMemory</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>afb11471e00117042c2bacfe74d49f0c8</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>parameterizedHandler_stripSaveCurrent</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ae6dfd29b6b2dd3941a4c1607ffce30fa</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>pending</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ad036a0da6036d9ba453d74111d8142ad</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>escapeSpecialCharsInJson</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a85e3000afaed04a831f832e1b0cf349c</anchor>
      <arglist>(String &amp;request)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>generateAsyncPageContentJson</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ae353b842ad6b146a693581e55aa1ed51</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleClientRequest</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>aa48264974edbdf407f1d32d0e909051f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>packNvmData</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ae7f91f6dce9a1827992ad0e87ecdddf2</anchor>
      <arglist>(uint8_t *nvmData, uint16_t length)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>printConfigPage</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ab5cf7a2d358d1e7b830dc8e21cd422fc</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>printSlotsConfigPage</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a9fab53955e7b2f27e6d72caec07ad6a3</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>processConstantRequests</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a2f47f87d2fabdeb1c2a5027d6c4be218</anchor>
      <arglist>(const String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>processLinkAsyncRequest</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ad358a77ca2191e996896a88e0dbc8c20</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processLinkRequestData</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>af432486a7b61e6adc75a2c5111f62c26</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>processParameterizedAsyncRequests</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>abc9c51f905b460ae12ac0148f67ce96b</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>processParameterizedRequests</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a2f2c788d36d8847a5243b48f1716ac0e</anchor>
      <arglist>(String &amp;request, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>restoreNvmData</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a29f117d7691619cc8435fdf915945cdb</anchor>
      <arglist>(uint8_t *nvmData, uint16_t length)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>currentTime</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a24b3dac7efeb057fbe8eca7b711cc7b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; DeviceDescription &gt;</type>
      <name>descriptionVector</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>aa58448a604695fb4acc5b35547d8d569</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::function&lt; bool(uint8_t, uint8_t)&gt;</type>
      <name>deviceBrightnessChangeCallback</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ac49bfab4926c13d2e8832e4c0bb0c342</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::function&lt; bool(uint8_t, bool)&gt;</type>
      <name>deviceEnableCallback</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a2926c383a7129c18d914a31aa3ec8b2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint8_t, std::vector&lt; DeviceDescription * &gt; &gt;</type>
      <name>deviceToRoomMappingList</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>adb5d48560d49dc618c63d5c5370de2c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static String</type>
      <name>header</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a6f26444649c03fe9f73a5e744fc5a667</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static String</type>
      <name>ipAddressString</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a87266751f19202d94b601766c4ac77aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static HttpServerNvmMetadata</type>
      <name>nvmMetadata</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ae127a5127030eabaa849c6ad485b2214</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>pos1</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a8113c086b9eaa2f4c6d69230d3e22df7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>pos2</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ae34dbf72c8a37a1058b9bfa4348beeaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>pos3</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a0074e9b4ed426e105fc44bd61a2fe445</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>previousTime</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ad252b86f1b445d3b0ef4ea1c53ccae31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint8_t, String &gt;</type>
      <name>roomNamesMapping</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a837d375f160968289c25eb9f6ad99a5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static SecurityAccessLevelType</type>
      <name>secAccessLevel</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a0a027e5ae8712dd6a8096abddd53b3bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static WiFiServer</type>
      <name>server</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>ad23c3c78fd55e448266d6f63aad903e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const long</type>
      <name>timeoutTime</name>
      <anchorfile>classHomeLightHttpServer.html</anchorfile>
      <anchor>a393e8db184d3404c283a109ea7a1740f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HTTPAsyncRequestHandler</name>
    <filename>classHTTPAsyncRequestHandler.html</filename>
    <class kind="struct">HTTPAsyncRequestHandler::AsyncHttpRequest</class>
    <member kind="function" static="yes">
      <type>static HttpErrorCode</type>
      <name>createRequest</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>ad28f5098355a1f141077a228e94f4861</anchor>
      <arglist>(AsyncRequestType requestType, uint8_t *requestParamsPtr, uint16_t paramsLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>getJsonResponse</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>abfcda8ff7c1950ffd06b95347f6191b5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static AsyncRequestState</type>
      <name>getProcessingState</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a5405b938acbe3d462e13610a4b4f5d72</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a4d9d2a20215938d7723d1d8117f94a22</anchor>
      <arglist>(std::map&lt; uint8_t, String &gt; *roomMapping, std::map&lt; uint8_t, std::vector&lt; DeviceDescription * &gt; &gt; *deviceToRoomMappingList)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>mainFunction</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a489f51899ac4e8266b9f78da524e7c27</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createDeviceConfigurationJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a5a8d207c1a037d215546f10b2ef92a0e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createExtendedControls</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a42ddfd29a798d9a7fbf808d7efdc27ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createHashJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a0d088a31eb802a9bc9638ae6067a059f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createJsonResponse</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>aaee7f30cef737c09b85429f4c2e94696</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createMainPageContentJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a2e24a40600b78e4c95ac0921243b0622</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createNotificationCountJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a0e3af3cb858b6abf108236175a0be7f2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createNotificationListContentJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a7615383e133bce7378823edcc3c8602f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createRedirectToMainJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a4e764c7d205e4ce159fe5c8603c9bc36</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createServiceCall</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>acab57e7dcf52fdfbcfa90ee88d1ac713</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>createSystemDetailsJson</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>ad9da6b26f4cc2ee2e5c1b79623696922</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>currentRequestClear</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a66f7e1225a1a45c7a64756b166509d38</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>downloadAdvancedControls</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a9d1e1821b9db4df178725603627947bc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processRequest</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a657d0cb592740893a038734f6b919967</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static AsyncHttpRequest</type>
      <name>currentRequest</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a622e23e608edbf4efd89e407d03f3ef4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint8_t, std::vector&lt; DeviceDescription * &gt; &gt; *</type>
      <name>deviceToRoomMappingList_ptr</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a96ab01c5e3acea69b330d4b1868cbd27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static String</type>
      <name>jsonResponse</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>a9b9d23fa8e76e06429455a337a8b35e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint8_t, String &gt; *</type>
      <name>roomMapping_ptr</name>
      <anchorfile>classHTTPAsyncRequestHandler.html</anchorfile>
      <anchor>ae87a1788848ae3c7d2aecf4596afb794</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HomeLightHttpServer::HttpServerNvmMetadata</name>
    <filename>structHomeLightHttpServer_1_1HttpServerNvmMetadata.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numberOfTranslations</name>
      <anchorfile>structHomeLightHttpServer_1_1HttpServerNvmMetadata.html</anchorfile>
      <anchor>a9d48ab70510fc22d052cdcd66e996a29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>restoredSuccessfully</name>
      <anchorfile>structHomeLightHttpServer_1_1HttpServerNvmMetadata.html</anchorfile>
      <anchor>a0d3d8b2a15f9940eb26b1b60bfd38814</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HwButton</name>
    <filename>classHwButton.html</filename>
    <base>Device</base>
    <member kind="function">
      <type></type>
      <name>HwButton</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a61fbed7dab683aceab082b67c59e484e</anchor>
      <arglist>(DeviceConfigSlotType nvmData, std::function&lt; void(uint16_t)&gt; localToggleCbk, std::function&lt; void(uint64_t)&gt; eventTriggerCbk)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>HwButton</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a4754855f6c44cd7258d12f0554265479</anchor>
      <arglist>(DeviceConfigSlotType nvmData, std::function&lt; void(uint64_t)&gt; eventTriggerCbk)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a7eddaa6028c55aee586f7628d06411b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>af10db870b798982a6070d3d378f670bd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a380eb8075842e185cf46dbf8f03c31dc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a0152bd7760a46560d190f42f0f21e423</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>aedb9a36c3d77fa9ae1b0a101f1b5417b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ac36372c6915dbcdfeaac3110d09c5edd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ad37659889fb0c9409f4d2c036f3e9ed9</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a1b5ca395d085d8c9fdc6ed8440a48cd9</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ad8b84e520b7873edee737568d6aed66b</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a94d56c503b0ccd44e8dadc51cd1f9be9</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void ICACHE_RAM_ATTR</type>
      <name>buttonAction</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ae254f1df6c6d64ce30ad8d3fec02346d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>processButtonAction</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a9bb4dfb3448f96f26dc292c4ca9b8930</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void ICACHE_RAM_ATTR</type>
      <name>interruptRouter</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a95fc4d01c321d0ea7d8757b3ed5a87c1</anchor>
      <arglist>(void *arg)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>activeHigh</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a4da20016e89e6dac30f44a2bbd77ce5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::queue&lt; std::pair&lt; uint8_t, long long &gt; &gt;</type>
      <name>buttonActionsQueue</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>aad047176582386e3fd1d2654b4a50f1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a3fb0bee99faf3994a62b9b924a503e64</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>doubleClick</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ac0325d9c3bc17f4bfdbb818a5410a72e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint64_t</type>
      <name>doubleClickEventId</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a634bac496ad18ab5d1a2b9c9c74ecd65</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const unsigned</type>
      <name>eventDurationTimeMs</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a671e0059eee6771da747c42bac6a2c74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>long long</type>
      <name>eventEndTime</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a427477686a78df20c2db89ac32d555f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>eventForceFinished</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ae79dfc3ea8c3390de6fb0250bdfa9c8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>eventStarted</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a39dabe5fe2477159ed11056882b86245</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>long long</type>
      <name>eventStartTime</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a492b09ad144608c601e861775ab78867</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; void(uint64_t)&gt;</type>
      <name>eventTriggerFunction</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a88e49e3f08b5834d2471f38dba12df0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>localDeviceIdToTrigger</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a95503556a8ff4d26299a63029518b259</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; void(uint16_t)&gt;</type>
      <name>localDeviceTriggerFunction</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ac552b38f2ce2e7c261015eb6d028be82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint64_t</type>
      <name>longPressEventId</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a5853f4e7ad00969ccfc82da79deb48a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint64_t</type>
      <name>optionalSinglePressEventId</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>a508db0f6495c13842a17eb9ebd222a87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>pinNumber</name>
      <anchorfile>classHwButton.html</anchorfile>
      <anchor>ae8fa5fb64b5aae2c82d74d74c4639b67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ILedAnimation</name>
    <filename>classILedAnimation.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ILedAnimation</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>ad393d66ee75c8208dfc43b451f360d36</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>a06b70bec7412a2056f4d661056a3b0ca</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isInProgress</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>a739494f74b41d482822bc7ecd3031d03</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>process</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>adda43083b8d0d1a11be66f0dd6fb2214</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>restoreColors</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>a3541a60a40ec8880083056772ab2de28</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>start</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>a70bc60ae87a0380b4dc314f9acfc772a</anchor>
      <arglist>(bool startFromZero)=0</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>isInitialized</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>aa2f36ccd1c9b325bcf58f5959c6e2813</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>LedColor *</type>
      <name>animationBuffer</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>af836f2bf89f1f69d6216057631a5e368</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>LedColor *</type>
      <name>colorValues</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>ad5c3a1ecc33dccd1b08d34680fd634ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint16_t</type>
      <name>ledsCount</name>
      <anchorfile>classILedAnimation.html</anchorfile>
      <anchor>a8390fac03b8f13a27acfd578f8c37172</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ILiveAnimation</name>
    <filename>classILiveAnimation.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ILiveAnimation</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a8b8a00ee36d6edd920e565b22ab48dc0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a92477a26aadfae07f5462fd78c7d62e2</anchor>
      <arglist>(LedColor *stripPtr, uint16_t count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isRunning</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a05b7525eb0f4b9774b9b97357bdbf7dd</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>process</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a986fb4b419e45cdb4917e5653249744b</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>start</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>aebdf84e35987b70661183a178faf632f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>stop</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>aecf96a94de608e09fc280d817dca5386</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>onStart</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a5d49612ae77d9767810f684540e94621</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>onStop</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>aea8f4d5ef264ed0686898ddcf7195a50</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>initialized</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a3c0fdeec19a53388ed8a3d45b7102965</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint16_t</type>
      <name>ledsCount</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a4454cb9aacf3b538c561d3a46ca93a30</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a79a94e36040f41040abc057a463958fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>running</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a810e11040091b5a13807b4de268fd9c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; LedColor &gt;</type>
      <name>savedState</name>
      <anchorfile>classILiveAnimation.html</anchorfile>
      <anchor>a0827631fa4db547f53329906d1791cf7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MessageUDP::IPAddr</name>
    <filename>structMessageUDP_1_1IPAddr.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>structMessageUDP_1_1IPAddr.html</anchorfile>
      <anchor>a05bd56939b31d41f333997b22fdddc1c</anchor>
      <arglist>(const IPAddr &amp;rhv)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>toString</name>
      <anchorfile>structMessageUDP_1_1IPAddr.html</anchorfile>
      <anchor>a500c172d240a0991b7a9ddda387a7c73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>octet1</name>
      <anchorfile>structMessageUDP_1_1IPAddr.html</anchorfile>
      <anchor>a0ad90f4b49e9504870a2a56a7decaa38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>octet2</name>
      <anchorfile>structMessageUDP_1_1IPAddr.html</anchorfile>
      <anchor>aafb30c905f5660d90588c97c990258d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>octet3</name>
      <anchorfile>structMessageUDP_1_1IPAddr.html</anchorfile>
      <anchor>a5b26f0f0dfc1d047977ee2535588f36d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>octet4</name>
      <anchorfile>structMessageUDP_1_1IPAddr.html</anchorfile>
      <anchor>a0330617aa1e0ce7d26fab8ee4f9cce8a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>KeepAliveData</name>
    <filename>structKeepAliveData.html</filename>
    <member kind="variable">
      <type>uint64_t</type>
      <name>mac</name>
      <anchorfile>structKeepAliveData.html</anchorfile>
      <anchor>a3d42290a083d148795c1f68502ff924b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>nodeHash</name>
      <anchorfile>structKeepAliveData.html</anchorfile>
      <anchor>a9de8841093cf92579ae3012b8961ee72</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LedColor</name>
    <filename>structLedColor.html</filename>
    <member kind="function">
      <type></type>
      <name>LedColor</name>
      <anchorfile>structLedColor.html</anchorfile>
      <anchor>ac960cda78dd99bf1e41453ce72cad4bd</anchor>
      <arglist>(uint8_t red=0, uint8_t green=0, uint8_t blue=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>structLedColor.html</anchorfile>
      <anchor>ab20c077b3ec76fe481eba538d5bf6526</anchor>
      <arglist>(const LedColor &amp;o) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>structLedColor.html</anchorfile>
      <anchor>a0250ca7e866efa51b55bc92005d97d22</anchor>
      <arglist>(const LedColor &amp;o) const</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>b</name>
      <anchorfile>structLedColor.html</anchorfile>
      <anchor>a3e9a3d5146e0fd245198bbe15736b9c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g</name>
      <anchorfile>structLedColor.html</anchorfile>
      <anchor>a7865e7ea907b705cc4eff661595e4829</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>r</name>
      <anchorfile>structLedColor.html</anchorfile>
      <anchor>af7fdc7ce91891f430753a3895c50ea23</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LedStripAnimationProperties</name>
    <filename>structLedStripAnimationProperties.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>animationSpeed</name>
      <anchorfile>structLedStripAnimationProperties.html</anchorfile>
      <anchor>a05dabe35b78dd2b72db5b1e74da8c6b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>disableAnimation</name>
      <anchorfile>structLedStripAnimationProperties.html</anchorfile>
      <anchor>a3e668d2a5beb8a6fa6a827e1b90b368c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>enableAnimation</name>
      <anchorfile>structLedStripAnimationProperties.html</anchorfile>
      <anchor>ad757a37e945279bd312f0654a8ddca88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>liveAnimation</name>
      <anchorfile>structLedStripAnimationProperties.html</anchorfile>
      <anchor>a93c0017da306a3dfb715b29741ed1597</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>liveAnimationSpeed</name>
      <anchorfile>structLedStripAnimationProperties.html</anchorfile>
      <anchor>a2f5bbbb8fcdb197820dc470b42e692d7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LedWS1228bDeviceType</name>
    <filename>classLedWS1228bDeviceType.html</filename>
    <base>Device</base>
    <member kind="function">
      <type></type>
      <name>LedWS1228bDeviceType</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a59116ab12376cd9f697f7eab6d8e0f7c</anchor>
      <arglist>(DeviceConfigSlotType nvmData, std::function&lt; void(void)&gt; reportNvmDataChangedCbk)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyColors</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a7534ad4a22e83b9ecdcb088907ac5864</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ServiceRequestErrorCode</type>
      <name>applyContent</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a32cb343423ad4ea83dc4da07ba9d2871</anchor>
      <arglist>(LedStripContentIndex contentIndex)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a667886fb07afdbc3fc2743f0ff736b77</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>getDetailedColors</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a078e18cb72c038357ad4bec3191b80cb</anchor>
      <arglist>(LedColor *memoryBuffer, uint16_t count)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ad5a22f5cdbf87dcd5dc47dbdb8aad5fa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a5c10a48753e63a6654ba8fb6e9c8a4e0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a76fb974b1f944341f71e7fa492b44bf1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>af3565bb3768b33fc099558df6fe4d1ca</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>adf6dd67b34abc6ddeba9fd05e6af3750</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isStripInitialized</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a9533408d30015fb625e53916003e870c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ServiceRequestErrorCode</type>
      <name>saveContentAs</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a16df20a58a5004ee2fbd854992fd1c96</anchor>
      <arglist>(LedStripContentIndex contentIndex)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a245642330ecd2b9e53e3e5b56e95bd56</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ab4ec80b67a4356935afc5afc3d4a34c9</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>acf59c2ba05b7713345e439080d594555</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ad9c8e1866db4ddab9777a64dd8678e5e</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setColors</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a7d9e7e6dc38d084217f4af6b183cbbd8</anchor>
      <arglist>(LedColor *ledsArray, uint16_t count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stripOff</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a58506f45b3bcdb6d2286faf355e463e9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stripOn</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a86ea29dafc8a5e5fc92bf652bcc94c73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>updateAveragedColor</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a563cfcca9acbaffe8dce6fd196d1f309</anchor>
      <arglist>(LedStripContentIndex content)</arglist>
    </member>
    <member kind="function">
      <type>ServiceRequestErrorCode</type>
      <name>updateExtendedMemoryPtr</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a66f98bc9b86f3dd5496e115390e81ab7</anchor>
      <arglist>(uint8_t *ptr, uint16_t size)</arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eACTIVE_CURRENT_CONTENT</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a379d9e206212ec1269fe250d801882cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eDIFFERENT_CONTENTS_COUNT</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a17adacccba7ef646d1ed64e1feded31b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT1</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a8665a0370fdb845e4f92982e71f173c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT2</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a99837474ce60e56d17dc8ed371fa8083</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT3</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a2579a1fd893207cd5c8c711ab9bb6bac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="private">
      <type></type>
      <name>LedStripContentIndex</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eACTIVE_CURRENT_CONTENT</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a379d9e206212ec1269fe250d801882cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT1</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a8665a0370fdb845e4f92982e71f173c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT2</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a99837474ce60e56d17dc8ed371fa8083</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT3</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a2579a1fd893207cd5c8c711ab9bb6bac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eDIFFERENT_CONTENTS_COUNT</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aebd6bc5c04d856ec2896ac812a664a56a17adacccba7ef646d1ed64e1feded31b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>applyVirtualToRealDiodes</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a97beaf63bcaad5bda96e1acff133fb3c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>createDisablingAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a199c2385ef834683891eea48361a3eb7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>createEnablingAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa9fd638b0523267054b6d9ecb3420563</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>createLiveAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a6dc90cf3f451593dd4475b5b3a14caa3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>setHwLedStripColor</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a3e11bb64715ea7276a22f5e5941c8261</anchor>
      <arglist>(uint8_t virtualLedIndex, uint8_t r, uint8_t g, uint8_t b)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>stopLiveAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>abbd92c20b929643c3e419540619ffa76</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>updateAnimationSpeed</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ad1df80e4ba5ea4dbae30071a895e72dd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Adafruit_NeoPixel *</type>
      <name>adafruit_ws2812b</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>af6fe6d443e292ee0c189a3f73cbaa2be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedStripAnimationProperties</type>
      <name>animationProperties</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a5bdcf92b74ad471974253ab05dffb589</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>animationWaitTicks</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a54bdeb10bffda12c7b1fd287678016d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor</type>
      <name>averagedColors</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ad27f9c22f27c3123e1b222bac2cd3261</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>brightness</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ac27fd51ebe109ff49cb0a13aa280533c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a93a6fb0f988034a9d7ce679a71cb21d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>String</type>
      <name>deviceName</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a9fa2212948ddca51133aef5b3022b02e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>diodesCount</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a9badbcd70837709e2807674b1b7e3312</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t *</type>
      <name>extendedMemoryPointer</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ac3e9f0f1a424775e6003442b244b3b33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isContentInitialized</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ad380fe6ae26e18c7b971b30653397972</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isInversedOrder</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a0104507e458ad9b0cf8dabc75c5040aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isOn</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>af36863fbafacc7f75de178f033e7a7ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ILiveAnimation *</type>
      <name>liveAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa31117853a28173b04b5aeba78a747dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>liveAnimationTicksToAnimate</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a6e9ea29d85f7b6e2dd6b85372391f566</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; void(void)&gt;</type>
      <name>m_queuedAction</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a863993a753395348f1b254c830471cf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; void(void)&gt;</type>
      <name>m_reportNvmDataChangedCbk</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a19af4c8ef0c32aa8fa63ab714685f900</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ILedAnimation *</type>
      <name>ongoingAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>adbadfb1e655c25235794ecd9db123df8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>physicalLedsPerVirtualLed</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a61f6b24a0e6467aa21d758f8318b24b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pinNumber</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ab5d99619bf0c88531e8841270b12d353</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ad0ac9f3cdd4ab84e337003f20da999dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>runtimeBuffer</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a8d1ee544ad9f1ecaae088b56f3b4d9c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>stripContent</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a416dc4cb728965429ee0f6f20a580a47</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ILedAnimation *</type>
      <name>switchOffAnimation</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a6fb8413715c9b3d0d0bfd18942d756e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>ticksToAnimate</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa4e371b66593a30e55defd8464890b91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>virtualDiodesCount</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>a44008e06abeef5e8895a652ba4c7fc8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>waitTicksLive</name>
      <anchorfile>classLedWS1228bDeviceType.html</anchorfile>
      <anchor>ab85867d3f4b59f7e8006a3263ea3290e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Logger</name>
    <filename>classLogger.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>log</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a099a7393789ba66f4997c3bfe16d9d15</anchor>
      <arglist>(String message)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processMessage</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a6e7f51adaadbe28d203d14e6b9c2b1a5</anchor>
      <arglist>(String &amp;message)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long</type>
      <name>logCounter</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a434749e8ef98c3a013b98ce23dc3bab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; String &gt;</type>
      <name>offlineLogQueue</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>afef4f4bd77d24b62120f6af4c58b4ce4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MessageUDP</name>
    <filename>classMessageUDP.html</filename>
    <class kind="struct">MessageUDP::IPAddr</class>
    <member kind="function">
      <type></type>
      <name>MessageUDP</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>aaa0b637ca95c570741092f11d375a4ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MessageUDP</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a671088ac52a9b760d6a9b4c4cf8d8779</anchor>
      <arglist>(int myId, MessageUDP::IPAddr addr, int port)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MessageUDP</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>ae3bf477f663733776c8c9c550e39a26f</anchor>
      <arglist>(int myId, MessageUDP::IPAddr addr, int port, byte *buffer, uint8_t bufferSize)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getCurrentByte</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a9e132058508b3114b83c949b0be2a9e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getId</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a35006643100efc50daa4fd9c62751c17</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MessageUDP::IPAddr &amp;</type>
      <name>getIPAddress</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a2380054734cff135e40c99b5714d3392</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; uint8_t &gt; &amp;</type>
      <name>getPayload</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>af71b7dc94c503448b0ecee93c861da88</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getPort</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a53e3aaa9912bcd671dff44f27b6b1bae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>getSize</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>afed36467e19698fc1bd12523a950ec11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a8b004a6cf8bb86b4b86b2c45eb2f1671</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>pushData</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a98392bf4febcffbdf6890ef86bba47d3</anchor>
      <arglist>(byte *arr, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>pushData</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>aabc8ce4fcb46676db6d1dc5e7feae29a</anchor>
      <arglist>(byte data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetByteIterationCount</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a80764cb636fcefcd60674a41188db33f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setIpAddress</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>aadaee9f16de0374384ebcfa74882613c</anchor>
      <arglist>(MessageUDP::IPAddr addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPort</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a4d89d3d60e97565b36865ae90ff36e1f</anchor>
      <arglist>(int port)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>switchToNextByte</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a216047478309cdf571206c054bc3847a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toByteArray</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a229eb2912db367526e10fec729240efc</anchor>
      <arglist>(byte *bufferPtr, size_t sizeCheck)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MessageUDP</type>
      <name>fromUint8Vector</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>aa5c6a5316e55fff77dfc5a9551444f63</anchor>
      <arglist>(std::vector&lt; uint8_t &gt; &amp;vec)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>serialPrintMessageUDP</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>ad632e88283d2afd6f853ffe2cae0589b</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>updateTotalSize</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a06750577d91a228785286821d174ee39</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>getMinimumSize</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a7e102b50023da14d5331f03b16ae3630</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>validateIpAddress</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>ab92762746803bcb7cce1ba22d21787c9</anchor>
      <arglist>(IPAddr &amp;addr)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>validateLastByteValue</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a328b2bcdf0f70dbc66e12bf5e47c0a13</anchor>
      <arglist>(uint8_t lastByteValue)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>validateMessageId</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>af3f27cdf3ab49287b4c632a83567ad73</anchor>
      <arglist>(int messageId)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>validateTotalSize</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a305d0a23736fc10588918ac93c8a83f4</anchor>
      <arglist>(uint16_t bufferSize, uint16_t tmpSize)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>byteIterationIndex</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>ab9a07e8316655ccfe3e21d49d5a3c4d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; byte &gt;</type>
      <name>dataBuffer</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a379183b2d801e9b3b33bbe04a90fc142</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>id</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>af9ed4f2b7bbf41a12ed806beb3d7f937</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>MessageUDP::IPAddr</type>
      <name>ipAddress</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a8cae6ceb134565043104f7d85839ce8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>totalSize</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>ae0ea90a40341c793d11185d5960d38b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>udpPort</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a88e53cdd857d0b408921271fc8ea4978</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const uint8_t</type>
      <name>lastByteCheckValue</name>
      <anchorfile>classMessageUDP.html</anchorfile>
      <anchor>a0a4970b79ee156d65ebdf19b61158c36</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NetworkDriver</name>
    <filename>classNetworkDriver.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>ad94a9254eb44e94ad7db81d54b104cf5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a4126eac581f05d84f511aa21d7b1884a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>ad7c174d9e2424575e925347adf1595e6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>networkReconnect</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a26853e557e191f81039d38eb77285547</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>runIpDetectionTrick</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a0f5d1d53c2892c41967889d4309692b4</anchor>
      <arglist>(String ssid, String pwd)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>send</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a11d1f15ed5e89abbc7b737cea20e98ea</anchor>
      <arglist>(MessageUDP &amp;data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>sendBroadcast</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a2509905b78d726a9be24f96319775f20</anchor>
      <arglist>(MessageUDP &amp;data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>udpReceive</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a8db312d13c899ca8135bec8cc006b884</anchor>
      <arglist>(MessageUDP data)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>mapReceivedPacketToInternalReceiver</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a9e6d57dc9a8c82a47cb5964bbfd59b04</anchor>
      <arglist>(MessageUDP &amp;packet)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>networkCredentialsAvailable</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a0766dff847d3614ddb983849ebd6c213</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; int &gt;</type>
      <name>packetRanges</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>adb5cd4089e79e6be82863ad7ca94acee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; std::function&lt; void(MessageUDP &amp;)&gt; &gt;</type>
      <name>packetReceivers</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a4216d7af8387b33b469e0fc33991a57b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::function&lt; void(void)&gt;</type>
      <name>pendingPacketsBehavior</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a4c9b26a218e3aa4b98a4ae4e66168cf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; MessageUDP &gt;</type>
      <name>pendingToSendPackets</name>
      <anchorfile>classNetworkDriver.html</anchorfile>
      <anchor>a86b29b8b21c32e2c0b51a05615a49011</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NetworkNodeInfo</name>
    <filename>structNetworkNodeInfo.html</filename>
    <member kind="enumvalue">
      <name>Master</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265faa36caa3b33636186f8f1f54bb98f3453</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>NodeType</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Master</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265faa36caa3b33636186f8f1f54bb98f3453</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Slave</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265fa45251d51b8eb07204fe87ce4437246b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unknown</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265faa74f6784e87d03209f7dc8d0c781b96f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Slave</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265fa45251d51b8eb07204fe87ce4437246b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unknown</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a0d1becb4de074114dc70424d2471265faa74f6784e87d03209f7dc8d0c781b96f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MessageUDP::IPAddr</type>
      <name>nodeIP</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>ac723b1d0b00ec1d21e8c343eb2a68564</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>enum NetworkNodeInfo::NodeType</type>
      <name>nodeType</name>
      <anchorfile>structNetworkNodeInfo.html</anchorfile>
      <anchor>a16663df79256728b6711d42da047ec7f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>NodeConfiguration</name>
    <filename>structNodeConfiguration.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>isHttpServer</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>ac876e7d8770dae2cfcfb8cf88fd20fb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isRcServer</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>a29e44123d1edcca7e593490522c9c2b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>networkCredentialsAvailable</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>a7eaea0aafa4482a22ec7637a936cd329</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>networkPassword</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>a3ba66f26dfff041b267a5a3887f790c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>networkSSID</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>ac474d173a459b568f7354f37a33a7791</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>nodeType</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>a4a62ae08d2f007d52b3fc6159e446312</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>panelPassword</name>
      <anchorfile>structNodeConfiguration.html</anchorfile>
      <anchor>ae43ce5b0b4f55b6d0fe6bf647420466e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>NodeInitialData</name>
    <filename>structNodeInitialData.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structNodeInitialData.html</anchorfile>
      <anchor>a97a8a364e998d9bb572ba90b6407be27</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>macAddress</name>
      <anchorfile>structNodeInitialData.html</anchorfile>
      <anchor>a13da89eff95de61becadcc1b34adeb85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>nodeHash</name>
      <anchorfile>structNodeInitialData.html</anchorfile>
      <anchor>ac71b7ed7d5b64314802240f0d108dd53</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numberOfDevices</name>
      <anchorfile>structNodeInitialData.html</anchorfile>
      <anchor>a3fac23a5b27345844933eaca74fddafc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NotificationHandler</name>
    <filename>classNotificationHandler.html</filename>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>createNotification</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>a984bd3f242981982d53079668b02a737</anchor>
      <arglist>(UserInterfaceNotification &amp;newNotification)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>a4cdcdb388bbec84da46f7b22ae51e2c6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>a4b1cc3177841ac4528095396b9430c36</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint8_t</type>
      <name>getActiveNotificationsCount</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>abf0626ad9fbff0758897aa1e159140a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static UserInterfaceNotification</type>
      <name>getOldestNotification</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>ab4228cdd8c379e171aa7bd58c80b8840</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>a04e05e51af10e7d0d067f6bfdd0cb5ff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; UserInterfaceNotification &gt;</type>
      <name>notifications</name>
      <anchorfile>classNotificationHandler.html</anchorfile>
      <anchor>a8d4b4114d9d2958dd1c59b3438f2300a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>OnOffDevice</name>
    <filename>classOnOffDevice.html</filename>
    <base>Device</base>
    <member kind="function">
      <type></type>
      <name>OnOffDevice</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a844317343454e106a4ad480329c4bae5</anchor>
      <arglist>(DeviceConfigSlotType nvmData)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>OnOffDevice</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a956702b0aead6f15ffb2e9bba56a6bcb</anchor>
      <arglist>(DeviceDescription &amp;description, uint8_t pin)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>OnOffDevice</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a4fd262d8c419fff840953c63c583e634</anchor>
      <arglist>(int pin, String devName, uint8_t a_deviceId, uint8_t a_roomId)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>brightnessChangeHandler</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a8f2744e297f0161468fa162d2b144520</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>changeBrightness</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a6237de69de530a1da1bd258872a742e4</anchor>
      <arglist>(int requestedBrightness)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a0779fea57ba3476b5c144e02a8103c1b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>getBrightnessIsAdjustable</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a7c30c5c1d47b91996b3d291b8d4827b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getBrightnessLevel</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a6c5d673e4df8171dfceedf5faa8ff234</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getBrightnessLevelTarget</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>af107c0b28c76114c230e7fe532c8930c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getBrightnessStep</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a3a2fbfd61fbc7c705a5d600535e752aa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getBrightnessStepDuration</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a9c67db108346027183f3209ba353f347</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a081e89d7edb76a0e2f92e7bb2cb32741</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getDeviceId</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a7bec656b2b7c6d5aae9b2ba028ce2162</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a18be7aff3ca06dc8aa18f964080989ac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a9eea95147e6c2812cba03d081593611f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a66a636923bb5837df6ca6db2e2dd5edb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getLightDurationTimerMS</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a2bd1f95d071c08b46c30b3cf06b91d8c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getName</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a05c779cc27d40ed5a26215c258c66222</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getRoomId</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ad9f812830c7e0dc3a5a67dd9f1dc2860</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>getState</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>acbf113e303093591b59c252008612db4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a0827eed810ac1d0c73619e2b571f815d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>mapBrightness</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ae9aadccea6c1bb05d52e3a8058259bb4</anchor>
      <arglist>(int brightness)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>off</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a81250e5425e077edf3b2f4c9579b1ad1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>on</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a6be0fcca5ab31a0296652c2e90e7b632</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a72e3bf8e47791f0e0dac67d3509342ac</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a5887396e62fedeb5142a0d314cafef73</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a8fd9e82f9395e91b032e36016987dafb</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ad3797bfdfaaac239a7b87841042e8635</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setBrightnessLevelSupport</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a1acb35c595d9bc98b69928aa3bffeca4</anchor>
      <arglist>(bool p_brightnessLevelSupport)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLightDurationTimerMS</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ab5ea41dd9e46d99a321bcf2dbc501d30</anchor>
      <arglist>(int requestedLightDuration)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timerHandler</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a766b35ef1059f6e2ad6df4236a7d63a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>activeLow</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ae8fd56302210f00adc595d6fa51307d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const int</type>
      <name>brightnessChangeTime</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ae0b30b02db16d1a611b94410835ada09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>brightnessLevel</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>aa78d8043d759aa639ddea3b14ce198a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>brightnessLevelBackupWhenOff</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>aeadeba3130917a08076564bfcf324092</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>brightnessLevelSupport</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a5697543e1ea3bc8a6f5c5a4c16d18aab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>brightnessLevelTarget</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>ac8a623b52c48f3dd09254164e7e14455</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>brightnessStep</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a25776d509fe07577872d457cca199632</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>brightnessStepDurationMS</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a809edc7237a8f5a7df3f54a900faa4aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>AdvancedControlsOnOff</type>
      <name>controls</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a425ee710a11807be2eaa8ae75b14d5be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a19cad25aea196a5fc489a16227ab5f10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>String</type>
      <name>deviceName</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>aad0d29fe9fbd9c3d845b5a179720d996</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isOn</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a02b3a2d4988bb3ba86e8a16ed0baeb0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>lightDurationTimerMS</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a9a728d3ebf866f070a2ade387d77aa00</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>maxPwmValue</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a16ebf1339bf0c08d27e7d56e797ff91a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>minPwmValue</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a5544500712141adb235078efbdcce434</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pinNumber</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a256a70a82813ce6ca4403890709e7029</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>aaf2d2bdde1694bef4cbf67a16c08cef4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>timePrevious1</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a0177c11012388f73ac2d3bb1d170c3af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>timePrevious2</name>
      <anchorfile>classOnOffDevice.html</anchorfile>
      <anchor>a287e1b95254c398cd690db3b770d5bd2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>OperatingSystem</name>
    <filename>classOperatingSystem.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>activateNvmSaveTimer</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a8bfaeedd1f2eb4cf74d56c69031f2125</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceInformation</type>
      <name>displayRamUsage</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a124f9cc14eaa5911296c777d87363022</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>af1a1883a749f5caeae71bd14589bfcde</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>requestSecurityAccessLevelChangeViaString</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a878ae8bf337bea4f1d1ddaa5d516bfde</anchor>
      <arglist>(String password)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reset</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>afb6e3158d63458d328cb46952bcb076f</anchor>
      <arglist>(uint16_t delay)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>saveNvmData</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a91e58918318bd789a0e59aeca80ab842</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task100ms</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>ae12ad3bcdce6779cbb1ded83a6fb16aa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task10ms</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>aa20a15e417e50aa5c9a84a4d6363c6ea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task1s</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a69e1cca1917bb4fabfb3ae81b6d0b5a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task20ms</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a362e49da5cc804107841472dea0f0f87</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task2ms</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>af5a77e5ec04c7dd960293792e8e0617d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task50ms</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a6134ad4de0f784959c73cfeed7633920</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>calculateRuntimeNodeHash</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a3b51a9790aac4f1374e005f16ec3ec34</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>changeSecurityAccessLevel</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a51122fa9597d20a80200fc97c64f5e3f</anchor>
      <arglist>(SecurityAccessLevelType newAccessLevel)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>detectHwMassEraseRequest</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a22164cedc215b60cbc8d8b4b1998a1a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleNvmSaveMech</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>ae74081aeca7d5888aaab575610457a71</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleSecurityAccessLevelExpiration</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>acc25ac86455a02a3e630506e26bdc405</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleUiBlockTimeExpiration</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a4ed4bb4dae26544ba008c30988739837</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>performReset</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>ae0fda26ac659d8284950d2cc190901dd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long long</type>
      <name>accessLevelGrantedTimeSnapshot</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a2e484aa4fc41e2d14b497e1a843fed6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static SecurityAccessLevelType</type>
      <name>currentAccessLevel</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a4017c86834e4baadfacb8d0e25529383</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>isHttpServerRunning</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a9ff3818746e8285aba330acc1b7d16c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>isNvmSaveTimerActive</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a2d1a766260d8c18c36be0cef302c2c07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>isRCServerRunning</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a55381e4967b2948f9b9eba5a648c80c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long long</type>
      <name>nvmSaveTimerValue</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a54fcd509987466ee3d892dcc09d026c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>resetCountdown</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a40b211ac8a7d1d66a8716e251e9b2516</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>resetPending</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>ad7406e43a338a1bc31291f4c6c7c4a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>runtimeNodeHash</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a4e5c14258dcc4a08577db9f4ccff72df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long long</type>
      <name>uIBlockTime</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a446af1bd92c99b5240c93c530c1f1d36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>uniqueLifecycleId</name>
      <anchorfile>classOperatingSystem.html</anchorfile>
      <anchor>a305323894415ae18fea504297ef9df13</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>OTA</name>
    <filename>classOTA.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classOTA.html</anchorfile>
      <anchor>a9c51ac8ec2ad91ac6b40cff64253cd74</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classOTA.html</anchorfile>
      <anchor>aca05358fda4739f3c2a782dc13f8015d</anchor>
      <arglist>(const String hostname, const String password)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>PersistentDataBlock</name>
    <filename>structPersistentDataBlock.html</filename>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>getSize</name>
      <anchorfile>structPersistentDataBlock.html</anchorfile>
      <anchor>a9ebfe66b5222fbef15bddc2a191595e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>structPersistentDataBlock.html</anchorfile>
      <anchor>a2b1385998dd9361a462ae0e7beff2129</anchor>
      <arglist>[50]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>PersistentMemoryAccess</name>
    <filename>classPersistentMemoryAccess.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flushExtendedMemory</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a449efb566eaad174238a1921e27a458b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a4ecddea23bc9c1a41f85340a3cd1341f</anchor>
      <arglist>(uint16_t eepromSize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>massErase</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a66bd33ede30cb3bd09a159001c810b53</anchor>
      <arglist>(uint16_t eepromSize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>readData</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a72a7f805b82116b71850f4a189e7db07</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>readExtendedMemoryWithOffset</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a51753858a97abae3aadcc74e5c4a5385</anchor>
      <arglist>(uint16_t offsetToStandardData, uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>saveData</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a3622ffe9de929c9753858f62eed8cb10</anchor>
      <arglist>(uint8_t *data, uint16_t size)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>writeExtendedMemory</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a5dbe555f0ba1abc926b38557196bc7ad</anchor>
      <arglist>(uint16_t offset, uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>eepromInitializedSuccessfully</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>a4e6927d717464a7fcef226d14db75730</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint16_t</type>
      <name>standardDataEepromSize</name>
      <anchorfile>classPersistentMemoryAccess.html</anchorfile>
      <anchor>aca11190c1afc0275bfa4df115e4d3929</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RcRequest</name>
    <filename>classRcRequest.html</filename>
    <member kind="function">
      <type></type>
      <name>RcRequest</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a46190e022d46d36ec75672db11e58c83</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RcRequest</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>ab57a4f48d85d237e69360c352af72b66</anchor>
      <arglist>(uint8_t targetDeviceId, uint64_t targetNodeMAC, uint8_t requestType)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>calculateCrc</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>ae400e2452a8c9a31f5658889d65d115f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>abb0a872eb3f1da47b981aa074a213655</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>fromByteArray</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a558f9028c0bb3c683d3cb4842b4316a3</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; uint8_t &gt; &amp;</type>
      <name>getData</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>ac1c79a02f916e8a7efeecd84e0b1c651</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getRequestDeviceId</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a6586be7e848cebe828f3b579dba0a26e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getRequestId</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a24a26e8cc975597f6f40edfa762b5d29</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint64_t</type>
      <name>getRequestNodeMAC</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a444670a6c5975c8b9be1c084e8d92e6a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getRequestSendCount</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>ac7ae446c7cf1cb748db9c6ae51346556</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getRequestType</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a6028aca91fce4176aa3b5b8bf695e9fb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>getSize</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>ae0a59284bf1d55accdfd05daccf9ea22</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>RcRequest &amp;</type>
      <name>operator=</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a224436947e3b6069328a58a3ae1f511e</anchor>
      <arglist>(RcRequest &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>ac5fb746be6d4ced28ecf60e224b9fdd0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pushData</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>afda06eb154a2977691a2e60c8884b37c</anchor>
      <arglist>(uint8_t *data, uint16_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pushData</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>afb2821472f92fed2d4e745f4779ccba4</anchor>
      <arglist>(uint8_t byte)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setID</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a98864c7c3e76420aa044008e56165340</anchor>
      <arglist>(uint8_t id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRequestSendCount</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a6d067474ff94c39dcde88e06737ca03b</anchor>
      <arglist>(uint8_t count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toByteArray</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a307d3e61d926746e8ea5d0f4f6072197</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>crc</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a701b119029e6fc91c26de71a0365be74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; uint8_t &gt;</type>
      <name>data</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a46e30b6a0c1a4a1d842da296cf562d85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>requestId</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a638ae87a598f2da9f956207501e5afe1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>requestSendCount</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>aafde1d4cdace41fa8c24c073d46686e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>requestType</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a173359cb3266bf6e46a1939d08405fa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>targetDeviceId</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a527ca68b87955ed0ed6b43b439027436</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint64_t</type>
      <name>targetNodeMAC</name>
      <anchorfile>classRcRequest.html</anchorfile>
      <anchor>a7b0edb1d33a1388001c8c0157a60648a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RcResponse</name>
    <filename>classRcResponse.html</filename>
    <member kind="function">
      <type></type>
      <name>RcResponse</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a0fac497b0403f9388d78b001ab538a12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RcResponse</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>ad3fd1a40a13a6dacded3b934c030fd2b</anchor>
      <arglist>(uint8_t respId, uint64_t respNodeMAC, uint8_t reqType, uint8_t respType)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>calculateCrc</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>af06ecf11f394aac5bed91ee7357f0c3b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>fromByteArray</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a60844f6f4ec290bab5e1310fec1baaf4</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; uint8_t &gt; &amp;</type>
      <name>getData</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a8de8a5eaf29c9851bd616227c20e8169</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getRequestType</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a5439837be5997ccf1fcb6058e702964c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getResponseId</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a321348e5b9815176a316ddc11d768243</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint64_t</type>
      <name>getResponseNodeMAC</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>ad64cff7dc49aaeea857a766e7164e176</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>getResponseType</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>aa0446d39b96aa45ab66f27cfaf3f5856</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>getSize</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>ad826bc4dffad0c437e33d1cab7ad4117</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a1e6315fb58ca88a507654b8c850f8372</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>aff40581409165de535ac3fddab7e8d31</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pushData</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>ac86d4bea0746b741ca800b6fb622a433</anchor>
      <arglist>(uint8_t *data, uint16_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pushData</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>ac5a3d688e6ca6a7156875ac289762da8</anchor>
      <arglist>(uint8_t byte)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRequestType</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>ad5bce889f2585d2320f924cbb99373ca</anchor>
      <arglist>(uint8_t reqType)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setResponseId</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a17451df1355b21fd248e1daf8e764747</anchor>
      <arglist>(uint8_t id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setResponseType</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a7db1e914bbb947d7b6f5a5f00a735896</anchor>
      <arglist>(uint8_t respType)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toByteArray</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a9de314b28bc9545f91425c813b7d349b</anchor>
      <arglist>(uint8_t *buffer, uint16_t size)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>crc</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a63e3f642c89a49d16b49d0a02429fb52</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; uint8_t &gt;</type>
      <name>data</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a5e84f250ee6a320b6470c19ccb61bbc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>requestType</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>af10f2151a25850624067042339df68c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>responseId</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a2a1c1cdb73e692c3181bc9f865e00132</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint64_t</type>
      <name>responseNodeMAC</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>aa415ed956e70a5d82bebca30455d9ed0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>responseType</name>
      <anchorfile>classRcResponse.html</anchorfile>
      <anchor>a36dd120b3baf31b63b45c42a76465aa8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RcResponseLong</name>
    <filename>structRcResponseLong.html</filename>
    <member kind="function">
      <type>uint8_t</type>
      <name>getSize</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a46f185f093678ff3fd29e5b4735af168</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a3d948cc9848a73c831882f87c0baeb6b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>crc</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a31d369ffb755ae2577c645204eddda09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a2379003f1b4f690213a04b30243d3ba3</anchor>
      <arglist>[100]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>requestType</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a1036e4a393255d685b5e5d4905063d7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>responseId</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a5f2f7e5310d3c0ffbfde599354f5e716</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>responseNodeMAC</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a8281a2ac9d3226e9fa9564faab522aeb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>responseType</name>
      <anchorfile>structRcResponseLong.html</anchorfile>
      <anchor>a7cffa70f7de3d4c8186981a248088ad3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RCTranslation</name>
    <filename>structRCTranslation.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structRCTranslation.html</anchorfile>
      <anchor>af6ed5fab6d1896a791913f4214863e92</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>structRCTranslation.html</anchorfile>
      <anchor>ad263932de438fc18b3144be5dac66029</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>mac</name>
      <anchorfile>structRCTranslation.html</anchorfile>
      <anchor>ab5adf031b945ab9bb14390e66582e5e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>onSourceNodeLocalId</name>
      <anchorfile>structRCTranslation.html</anchorfile>
      <anchor>a248201d145c15d6544ff3db4f0cacd7b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RemoteControlClient</name>
    <filename>classRemoteControlClient.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>ab3eae89c140bd69e2e78de3a6fbccce9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a48ae630a25c37eca293f93523abc9a70</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a17f787f265af0df31b6bf705817a5b74</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>receiveUDP</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>aa696a8cf63bd09a107f9668879302f5e</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>registerRequestReceiver</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>af19f7e9b0bf82f6435921404fa5496e1</anchor>
      <arglist>(RequestType request, std::function&lt; bool(RcRequest &amp;)&gt; receiverCallback)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>sendResponse</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>ad3e2f848abb4d846a4b159bd27d72268</anchor>
      <arglist>(RcResponse &amp;response)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleKeepAliveState</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a06645c129a3c3b1e7ce3ad2fc2959cad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleNodeDetailedDataState</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a0d4b2ae02674bd1398bd46d4062c84c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleNodeInitialDataState</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a4d0a991bfdfcd86bb9ac97d8150f9ba5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processGenericRequest</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a8a0bd3201247a89da7356c8b8430ed94</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processPendingTxData</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a6a02cec5aa74db8decd6c3881ce7f8a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>processResponse</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>afc758bc6ba9f4551d465d314e1808ff0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processUDPRequest</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a4911f782e6bc1b6a3738ac9a193d3f80</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>sendDetailedDataResponse</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>add33197fccb43fa97c43f5293ae20990</anchor>
      <arglist>(UdpFrames_RCS udpHeaderValue)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>sendInitialDataResponse</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a41bd9023416e47b9d8236f32d4424978</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>sendKeepAlive</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a5f6a5729d293fd18b0211c06835b3c45</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateLastKnownMasterIp</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>af920c8215fadbb26e2086d0a46915577</anchor>
      <arglist>(MessageUDP::IPAddr address)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static Transaction</type>
      <name>currentTransaction</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a406000aed313025ebf30f845a5aa8695</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static MessageUDP::IPAddr</type>
      <name>lastKnownMasterIp</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a429aad06664b3f34ae935a7339209890</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>lastMasterActivityTime</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a1898cb30de0ea87e2f5437a4e81bc165</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>lastReceivedRequestId</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a5f18db9be916dda9f3173f54b3529eb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint64_t</type>
      <name>localNodeMACAddress</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a069323d2661ae1a739db483103872c87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; MessageUDP &gt;</type>
      <name>pendingTxQueue</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a5b421b6c9a069c8087935f5054f45d27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; MessageUDP &gt;</type>
      <name>receivedBuffer</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>ad42823bb4b88e92429ebd5e8fa5910bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::array&lt; std::function&lt; bool(RcRequest &amp;)&gt;, REQ_COUNT &gt;</type>
      <name>requestReceivers</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>a6d367c36666b19e269f97e74773a52e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; RcResponse &gt;</type>
      <name>vecResponseMessage</name>
      <anchorfile>classRemoteControlClient.html</anchorfile>
      <anchor>ab726018ec641df3866cdb3f847362f3a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RemoteControlServer</name>
    <filename>classRemoteControlServer.html</filename>
    <member kind="function" static="yes">
      <type>static uint8_t</type>
      <name>createRcRequest</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a97898d4aac2a0e8ae738431016672d00</anchor>
      <arglist>(RcRequest &amp;newRequest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>aa4e68877027648881e3056a61f40e533</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a62318cb27c613a0490d3eb0d247f3b21</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a11dcc3062e74550f1130b33d4d70d277</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>receiveUDP</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>ab3946b62b2ea3853b0f603f8f4d30f4d</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>refreshRemoteNodeInfo</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>ac75eaff96966b6ea6bfc3e4d55b756d2</anchor>
      <arglist>(uint64_t mac)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>registerResponseReceiver</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>ab58c516f7e4956cf8f8f336664cd2f02</anchor>
      <arglist>(RequestType request, std::function&lt; bool(RcResponse &amp;)&gt; receiverCallback)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>updateSlaveInformation</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a22b4325a2275c41690c8d3f5061fd07a</anchor>
      <arglist>(DeviceDescription &amp;deviceDescription, uint16_t newNodeHash)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>generateRequestId</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>af18e7d6a8df158cfed10e90777cba9fc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static NodeInitialData</type>
      <name>getInitialDataFromPayload</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a50f30754f5dbca6f7bf058ab71a79196</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleDetailedDataRefreshMech</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a74ff94b30d95664520f4a0dbc4789de1</anchor>
      <arglist>(std::vector&lt; uint64_t &gt; &amp;nodesToBeRemoved)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleDetailedDataUpdate</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a040fbbb490737ac85d55c52c3446ce8c</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleHandShakeCommunication</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>abe0b99abbec00e5dab4c6470a3df66fa</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleKeepAliveState</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a5efea8b514d2d86d094453fed8cab69b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleRequestNodeDetailedDataState</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>af6353506571180693a4305d487773cd2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleRequestNodeInitialDataState</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>aaa34a0b06b08052e8f6b8412a95b4156</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleSlaveAliveMonitoring</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a97f6c20f1240b5a15a93574dc48ce2c0</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static ServerState</type>
      <name>mapMsgIDToServerState</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>ad0b41cc52efcf6648f4d6ce217f34507</anchor>
      <arglist>(int msgID)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>processPendingRequest</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>ab39db1d4d15bb748a362a7d77c508675</anchor>
      <arglist>(RcRequest &amp;request)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processReceivedRcResponse</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a54bd05fd8eadcb20e2026aec2fba21a8</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>processUDPMessage</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>aeb383de9a3e1e57ca5ca2ecf0e715a9e</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>requestKeepAliveData</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a8c518ce703cf12bd98035c99d331d466</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>requestNodeDetailedData</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a0f2f7824d3823a3b70f76374c00c8889</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>requestNodeInitialData</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a017e232a14de008859af9b6ea5f882ac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>triggerDDRefresh</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a454b56a1a72c9b44911b521814f5c03b</anchor>
      <arglist>(uint64_t mac)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateDeviceDescriptionSignal</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a43dbfc1c1ac991778bea7ead8cce0e93</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateNetworkNodesInformationSignal</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a8f77dc4f545f6c421b4a3be8c4a3ef15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ServerState</type>
      <name>currentState</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>abc700439dbf2a31d1a35c11b81b74a73</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint64_t</type>
      <name>detailedDataPendingNodeMAC</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a55e30304fc458c0f3868d7879d10c45e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint64_t</type>
      <name>lastKeepAliveRequestTime</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>af3266b9acebf8c835e26b6c1051e08da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; uint64_t &gt;</type>
      <name>pendingDDRefreshNodeMACs</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a3f617a6c1134b597b9235380a6543337</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; RcRequest &gt;</type>
      <name>pendingRequestsQueue</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>aa0f9beeeee915c179482d36c0ce0439f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::queue&lt; MessageUDP &gt;</type>
      <name>receivedBuffer</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a512cc9466167284123d75a0232db1b37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint64_t, RemoteNodeInformation &gt;</type>
      <name>remoteNodes</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a98d846b5c9126b031fb5865f90ee4001</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>requestIdCounter</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a114c8afad53d98f22866449556ae3a2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static RequestProcessor</type>
      <name>requestProcessor</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a895ccdafd71d9d31ceb6e5b818a36abf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::array&lt; std::function&lt; bool(RcResponse &amp;)&gt;, REQ_COUNT &gt;</type>
      <name>responseReceivers</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>a518dc72f0f03c710e2345eb21d1870b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>slaveMonitoringBlockedDueToRequestProcessing</name>
      <anchorfile>classRemoteControlServer.html</anchorfile>
      <anchor>af95646624ba478a24bdc9030412fc7af</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RemoteDevicesManager</name>
    <filename>classRemoteDevicesManager.html</filename>
    <class kind="struct">RemoteDevicesManager::ServiceCallFingerprint</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>ae361f0d268defbd45ebc3d4d66bd5cdc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a7a20507e2771cf798a69f386b7c87b34</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static RCTranslation</type>
      <name>getTranslationFromUnique</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a5260a34824e778d9e1e38c0f2052feb2</anchor>
      <arglist>(uint8_t uniqueId)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a33f96f033a0940dee6e4a1c8773da758</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>printTranslationMap</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a572f3427dda01890a414a16458329f69</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>receiveResponse</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>acaa2165782211092d164c43ff0139118</anchor>
      <arglist>(RcResponse &amp;response)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>ab013aa871fe1ebb64924e02695901ea5</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>ac903f441a82f28364cd4e84ff099aadb</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>af6273bb079383a5d0e463e3704e26f44</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a3785ed3fba8f844e828b517f6df7e9bb</anchor>
      <arglist>(uint8_t deviceId, DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>tunnelDataUpdate</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>ad2081a36e441589ed95388ffac3be666</anchor>
      <arglist>(std::any remoteDevices)</arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_NO_REQUEST</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1a97a26eb26de1f2e047bbf53a6a828c2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_REQUEST_COMPLETED</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1af66d3da13d951431bb8fed7c0d663a2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_REQUEST_FAILED</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1aaa447425553f3ecba4654dd4567a6648</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_REQUEST_IN_PROGRESS</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1a5e52e325e699ab2747cf80f8bc92e988</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="private">
      <type></type>
      <name>RDM_RequestProcessingState</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_NO_REQUEST</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1a97a26eb26de1f2e047bbf53a6a828c2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_REQUEST_IN_PROGRESS</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1a5e52e325e699ab2747cf80f8bc92e988</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_REQUEST_FAILED</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1aaa447425553f3ecba4654dd4567a6648</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>RDM_REQUEST_COMPLETED</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a6a24f48360ac3ac9b95a331eb694cdb1af66d3da13d951431bb8fed7c0d663a2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>getMappingOffsetForNode</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a5f22156168cc34c6b8de0311f7b97345</anchor>
      <arglist>(uint64_t &amp;nodeMAC)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleService3Response</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>aaacc8f7a8cff48787d632db47ade6dfa</anchor>
      <arglist>(RcResponse &amp;response, DeviceDescription &amp;responseDeviceDescription)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>awaitingResponseId</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a208f4609990d12c6c71c932c80ea550d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::map&lt; uint8_t, RCTranslation &gt;</type>
      <name>currentIdMapping</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a968016ed467b1485999648d1d77efbac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ServiceCallFingerprint</type>
      <name>currentRequestFingerprint</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a2f56c600f21197c8e80830a1c3eac338</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ServiceRequestErrorCode</type>
      <name>currentRequestRespErrorCode</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>abd8ade6756b0406950051918549d2913</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::array&lt; ExternalNodeMapping, 24 &gt;</type>
      <name>mappingSlotsForExternalNodes</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a39227bc78e6cd8a8ee1ff7c52aafebc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::vector&lt; DeviceDescription &gt;</type>
      <name>remoteDevicesCollection</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a002db31ed4e13c024171563ff46d131d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static RDM_RequestProcessingState</type>
      <name>requestProcessingState</name>
      <anchorfile>classRemoteDevicesManager.html</anchorfile>
      <anchor>a1bda8615c4648b10d6832eec5e377e32</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RemoteNodeInformation</name>
    <filename>structRemoteNodeInformation.html</filename>
    <member kind="function">
      <type>void</type>
      <name>printLn</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>aa220fc47fb6c29740b0c606f06493cad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; DeviceDescription &gt;</type>
      <name>devicesCollection</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>aac1d3ddd50210fdcbf501cb614a92880</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isDeviceCollectionCompleted</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>a235ec769b95f39bfc343afa37284474e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>lastKeepAliveReceivedTime</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>a83d39cef5de204b81d0d8539ef27fea0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>lastKeepAliveRequestedTime</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>a1c8628e624318a6035b2775b62de98e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>lastKnownNodeHash</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>a872cf32be7a40a1d0a39c74f6e7ef015</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MessageUDP::IPAddr</type>
      <name>nodeIpAddress</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>a58345865e4a89267d780888b21f3df99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numberOfDevices</name>
      <anchorfile>structRemoteNodeInformation.html</anchorfile>
      <anchor>a4cb0c1f97b0babc7ac83b8fbe8a046c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RequestProcessor</name>
    <filename>classRequestProcessor.html</filename>
    <member kind="function">
      <type></type>
      <name>RequestProcessor</name>
      <anchorfile>classRequestProcessor.html</anchorfile>
      <anchor>a52a7733b2fc94e70af655ad0923a0060</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>processReqest</name>
      <anchorfile>classRequestProcessor.html</anchorfile>
      <anchor>a8950a8da97b545ff838bc1a86642c33c</anchor>
      <arglist>(RcRequest &amp;newReqest, MessageUDP::IPAddr targetNodeIp)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RcRequest</type>
      <name>currentRequest</name>
      <anchorfile>classRequestProcessor.html</anchorfile>
      <anchor>aa6fe24b1e637a416d274d88b9f488006</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isProcessingStarted</name>
      <anchorfile>classRequestProcessor.html</anchorfile>
      <anchor>a379db07c2db38ce2d1e1d3bbd1a5da70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>lastSendTime</name>
      <anchorfile>classRequestProcessor.html</anchorfile>
      <anchor>a612454289334dffbb7555381bd607444</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RFButtonInfo</name>
    <filename>structRFButtonInfo.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>structRFButtonInfo.html</anchorfile>
      <anchor>a3a70fc4d1246e72a94d4853418c480d0</anchor>
      <arglist>(const RFButtonInfo &amp;other) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>structRFButtonInfo.html</anchorfile>
      <anchor>a7d28408569aab3a09b8e2ebc44a60f5d</anchor>
      <arglist>(const RFButtonInfo &amp;other) const</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>bits</name>
      <anchorfile>structRFButtonInfo.html</anchorfile>
      <anchor>a82a7e2d47e2a75b79362621a158d6638</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>buttonUniqueNumber</name>
      <anchorfile>structRFButtonInfo.html</anchorfile>
      <anchor>a9889981f38b4a419629d93bbd9370f0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>protocol</name>
      <anchorfile>structRFButtonInfo.html</anchorfile>
      <anchor>a306adc82f4c4d0928799ec4cf10d5a19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RFButtonsControlAPI</name>
    <filename>structRFButtonsControlAPI.html</filename>
    <member kind="variable">
      <type>std::function&lt; void(uint32_t)&gt;</type>
      <name>deleteConnectedButton</name>
      <anchorfile>structRFButtonsControlAPI.html</anchorfile>
      <anchor>aa28a5f69a2f1bdb6d1780a26917a372a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; void()&gt;</type>
      <name>resetConnectedButtons</name>
      <anchorfile>structRFButtonsControlAPI.html</anchorfile>
      <anchor>afba7c1f8a3ab401ca8d183465a22559e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; void()&gt;</type>
      <name>startLearningMode</name>
      <anchorfile>structRFButtonsControlAPI.html</anchorfile>
      <anchor>a353b1686254cf56f79af702e34089077</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RFManager</name>
    <filename>classRFManager.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a206a46fe8904a487eb7b57f729764f46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>aec004aa81eef13c0c53c1ff02ab93caf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deleteConnectedButton</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a101696920dff0537d356e6100e32a13e</anchor>
      <arglist>(uint32_t buttonUniqueNumber)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a7092645103548a4f03690ea14325a2c1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>resetConnectedButtons</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a1d32d5a276b6b9a504a761120927fa80</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>startLearning</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a64b7c5a165dcd480f539e6079a06b0aa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>evaluateButtonEvent</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a1d462ed3b8bca49cc9721b6bcc59df5a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleIdleState</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>afb56599dece703155561b36125e43beb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleLearningState</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a997ab82be6c33b1d211438cc64a8c838</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static RFButtonInfo</type>
      <name>receiveButtonSignal</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>ac6a98b12cd514c7fb76f8a158627249f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>resetReceptionEvent</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a8536bfc4de693fbe7e9b6eb5791f9aa9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>restoreConnectedButtons</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>aad569932c3fad98fd6212d7bc9a1f402</anchor>
      <arglist>(uint8_t *nvmPtr, uint16_t nvmSize)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>saveConnectedButtons</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a7d4567f18adb83a0320fba3202e6e078</anchor>
      <arglist>(uint8_t *nvmPtr, uint16_t nvmSize)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::set&lt; RFButtonInfo &gt;</type>
      <name>connectedButtons</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a59f0c89a153695c85fcd98bc488b441a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static RFReceptionEvent</type>
      <name>currentReceptionEvent</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a56862d59939349fc070465479b5000e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static RFManagerState</type>
      <name>internalState</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a43d8976ad1c37d6e3283723b1ea0aa9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>lastEventCompletionTime</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a94cc25f50f3789a06f317ccb33cf4c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>learningStateStartTime</name>
      <anchorfile>classRFManager.html</anchorfile>
      <anchor>a93a0d32f8a7533657d2c54cfce0ace38</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RFReceptionEvent</name>
    <filename>structRFReceptionEvent.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>buttonUniqueNumber</name>
      <anchorfile>structRFReceptionEvent.html</anchorfile>
      <anchor>a8509f19b4645e03baa880ce9400581d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long</type>
      <name>endTime</name>
      <anchorfile>structRFReceptionEvent.html</anchorfile>
      <anchor>a1af9ed740f95143f2019eaf6a474d4ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long</type>
      <name>startTime</name>
      <anchorfile>structRFReceptionEvent.html</anchorfile>
      <anchor>a991d22f83d0715b10eefe88812478398</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RollInAnimation</name>
    <filename>classRollInAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>RollInAnimation</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a72658d56969e86afa97a30cadce15101</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount, Direction dir=Direction::LeftToRight)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RollInAnimation</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>ad1414032bcbeff048febd9f872727470</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>aee9608980e59e85b56dbcf32720b96fe</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isInProgress</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a4476f987396a2ef7b14a7390d6c37882</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>ad0df93db1160ab2d16485318ffd1cd4d</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreColors</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>ad9c56344e083498efee182f668d73a9c</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a21b1a4ff1f31a7efe3867bca5f794fcc</anchor>
      <arglist>(bool) override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>currentPos</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a645245cec6fbb015197c6f9082ea2fd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Direction</type>
      <name>direction</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a5f011d4e7118c083b1df6b9ad641f686</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a488eb9b80b1e4d52d26dbb0fa823c6c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classRollInAnimation.html</anchorfile>
      <anchor>a046711a9cba8b3a8c6c8f98f2a5d3f86</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RollOutAnimation</name>
    <filename>classRollOutAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>RollOutAnimation</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a850c6a6d496c3855f7006b30ffd2d5a4</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount, Direction dir=Direction::LeftToRight)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RollOutAnimation</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>adfdeb455e81d5798dba0ca1b63f187f5</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a64192a9f36b585f062e1c9d9dda16ae8</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isInProgress</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a330726ec6340690eaed34e37af3988bd</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>ab6f46712072429ed65b43d3742e3e3bc</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreColors</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a9a50b033e055346c80972b234d61289f</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a3451a63042a4629a184928e7490dccdc</anchor>
      <arglist>(bool) override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>currentPos</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a1b7743d8fd9ed826f7f0fba397fd7600</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Direction</type>
      <name>direction</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a3ad7540b1f01f3b6dbccb3b33a667eb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>ad18d2ba0b98491a73ffb09e7b663f176</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classRollOutAnimation.html</anchorfile>
      <anchor>a8db94e83148f3d96d75c9d6ffe4889fb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RtcTime</name>
    <filename>structRtcTime.html</filename>
    <member kind="function">
      <type>String</type>
      <name>toString</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a8c2f9462a335e5706adff8b1e2a0d261</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>hour</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a30ce3ff875744075428d47138e418dd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>isdst</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a27ee50be0a6fe12e77bd2865aa6c3315</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>mday</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a8485bcabd3d071a118ea120bdeaa5f29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>min</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a22122b695e191eb95feb6104b0625964</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>mon</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a5e7f862ac393e72163b3e77dada25ab3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>sec</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a5078910f1f5bac30ab0775b752f6cb5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>wday</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>a9648fc47c35b886601f667244aea3eff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>yday</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>aeab11aae36e49f45abcb15c21961519a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>year</name>
      <anchorfile>structRtcTime.html</anchorfile>
      <anchor>aa3e17764e1f1760fe6cc441a8be80656</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SafeResetManager</name>
    <filename>classSafeResetManager.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>a82dafcd5b51837aee362f9924cae8e7d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>a8c15b39c357f20f8430d18f0f1f0fe96</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>a0d9317eb08b78dbbb69549784358fa2e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>isMorning</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>ab4b2d2a5961359db69c762af38982f34</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>localDevicesConditionFilled</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>abe422adb03a60fc8074481d1905f3c90</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const uint8_t</type>
      <name>morningEndHour</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>a21c7728cd0ac88ded2fe2976b91323af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const uint8_t</type>
      <name>morningStartHour</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>a365b3f2fd3dd7dee0112e9afa6aeee3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const long long</type>
      <name>RESET_DELAY</name>
      <anchorfile>classSafeResetManager.html</anchorfile>
      <anchor>adec6b6bf31f982f66fc5f72ab5b098ee</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SegLedWS1228bDeviceType</name>
    <filename>classSegLedWS1228bDeviceType.html</filename>
    <base>Device</base>
    <member kind="function">
      <type></type>
      <name>SegLedWS1228bDeviceType</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a31411c41823c4c6c6387ddcd1c1fb89e</anchor>
      <arglist>(DeviceConfigSlotType nvmData, std::function&lt; void(void)&gt; reportNvmDataChangedCbk)</arglist>
    </member>
    <member kind="function">
      <type>ServiceRequestErrorCode</type>
      <name>applyContent</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a2a6058aaec51a4004a0ee1500f92153f</anchor>
      <arglist>(LedStripContentIndex contentIndex)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>af67e7fdf4cb747eb714dc210cdb9d565</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>getDetailedColors</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>adaac33c3b90cba1ed22bd6fe4b90fa9a</anchor>
      <arglist>(LedColor *memoryBuffer, uint16_t count)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a0666c69fe1ac6d67318e95051a3495d2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a271d0942774fc9342a09f684bb064299</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a88718c2775dffaad9a0890bf97535ee2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a3a916e75c3d30b34e2ae9de19cc58c8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a5489a201aa8ad39a627275d5badd65de</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isStripInitialized</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>af7e0335ff4333a2a73ebae374bacd47e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ServiceRequestErrorCode</type>
      <name>saveContentAs</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a96b903a9f5d91a17574ba90ba2e1999d</anchor>
      <arglist>(LedStripContentIndex contentIndex)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a8c6d993bb2a02a53f9b1df5326ce8cee</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a548855170fa09f96d08a01a949e21e1e</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a9c705e625bb66a79db46c31e0418674c</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a891396e28ecba42dee124154ffe9d1e6</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setColors</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a9d0a90545c87f6a56fa47afe90536dce</anchor>
      <arglist>(LedColor *ledsArray, uint16_t count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stripOff</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a410d44f1796a29f6d3df7107ab3858f4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stripOn</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a7e11182d4304cfe11efff6e3a61c1a7b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ServiceRequestErrorCode</type>
      <name>updateExtendedMemoryPtr</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a74531121852f7abc1a09377c347dccef</anchor>
      <arglist>(uint8_t *ptr, uint16_t size)</arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eACTIVE_CURRENT_CONTENT</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2a8325bb80798d3029f9faabde5f0d2416</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eDIFFERENT_CONTENTS_COUNT</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2ac79d87f40755ba67e0ac301af736727b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT1</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2a8e80dbc23a941bd9976c8293437bd5ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT2</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2a6cbd35ea8b983be067e66368ba5ef726</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="private">
      <type></type>
      <name>LedStripContentIndex</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eACTIVE_CURRENT_CONTENT</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2a8325bb80798d3029f9faabde5f0d2416</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT1</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2a8e80dbc23a941bd9976c8293437bd5ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eSAVED_CONTENT_SLOT2</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2a6cbd35ea8b983be067e66368ba5ef726</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="private">
      <name>eDIFFERENT_CONTENTS_COUNT</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aa83e18eb8514afbb82226c64b64e59a2ac79d87f40755ba67e0ac301af736727b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>applyVirtualToRealDiodes</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a7b932478184e6d784c753c6057244d23</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>setHwLedStripColor</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a63a25dafeb60eb00933d5e57c27b4e1d</anchor>
      <arglist>(uint8_t virtualLedIndex, uint8_t r, uint8_t g, uint8_t b)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>setSegmentState</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a1f5987ae0de9fe5c3c8123d8fe1bb4b6</anchor>
      <arglist>(uint8_t segmentIndex, uint8_t state)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Adafruit_NeoPixel *</type>
      <name>adafruit_ws2812b</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>af8eea0281528f29bf552255dbb264a08</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor</type>
      <name>averagedColors</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aae55f8343f2004718977a92e25d2f39d</anchor>
      <arglist>[eDIFFERENT_CONTENTS_COUNT]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>brightness</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>ab4c8ad71d60a672faad5488d38596788</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>ae2bc0a516d396793b710ba92f68a432c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>String</type>
      <name>deviceName</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a41ded484d0e219e127743e726c26d902</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>diodesCount</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>addc19353c87b3e7680a59238e1afbcd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t *</type>
      <name>extendedMemoryPointer</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>ae9934ed1afb7875519c006e7b429568f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isContentInitialized</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a000c8df1e5c3855e58347cc2419507a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isInversedOrder</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a9e84a2bb8ae88eecb7d7458eba6da5f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isOn</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a81b51f63e668c3d76ea527da7e0306c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; void(void)&gt;</type>
      <name>m_reportNvmDataChangedCbk</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>aeaceb0c649cfa8812f622ac2bd964ea4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ILedAnimation *</type>
      <name>ongoingAnimation</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a64f51db9742c3a8bb3154f97c516013a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>physicalLedsPerVirtualLed</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>abe8a837a045b8bb288862def00ace572</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pinNumber</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a364273b6a3dfc74a0ddd0505296bd4a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a50c52491140becf640e493aa97a5cf34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; LedColor &gt;</type>
      <name>segmentColors</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a965d85f2a998c9bd042b3f1dc322063a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; uint8_t &gt;</type>
      <name>segmentFlips</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a2c28cedffd7551d082ef50f443e0821c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; uint8_t &gt;</type>
      <name>segmentLedCount</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>ae155475659e9b41b25f7d3e70dead60c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; uint8_t &gt;</type>
      <name>segmentStatus</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>adef58f112195afa7de7677f78bdf0c84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; SegmentData &gt;</type>
      <name>stripContents</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>ade3b2b6a0e94812d11f9f6bbb9c1c929</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ILedAnimation *</type>
      <name>switchOffAnimation</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a435f9d3038809633c793a4c7187406e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>totalLedsCount</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>a964f458e7f05cc4b20ca982e58e61a63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>virtualDiodesCount</name>
      <anchorfile>classSegLedWS1228bDeviceType.html</anchorfile>
      <anchor>acdbdf6ecc0b6863cff57d332c958d684</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SegmentData</name>
    <filename>structSegmentData.html</filename>
    <member kind="variable">
      <type>LedColor *</type>
      <name>current</name>
      <anchorfile>structSegmentData.html</anchorfile>
      <anchor>a0682e84989ef6f0c8d23f3976f7910f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LedColor *</type>
      <name>savedSlot1</name>
      <anchorfile>structSegmentData.html</anchorfile>
      <anchor>a29f8d1d4abc202abf57b8e239f75980d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LedColor *</type>
      <name>savedSlot2</name>
      <anchorfile>structSegmentData.html</anchorfile>
      <anchor>ab9235eb424805baddfd7374334a1d15a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TempSensorDHT11DeviceType::SensorReading</name>
    <filename>structTempSensorDHT11DeviceType_1_1SensorReading.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>humidity</name>
      <anchorfile>structTempSensorDHT11DeviceType_1_1SensorReading.html</anchorfile>
      <anchor>a8785f18e87375dd6b963e7d44ebd9a0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structTempSensorDHT11DeviceType_1_1SensorReading.html</anchorfile>
      <anchor>abb16d2b10e070d75212222918885bae0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtcTime</type>
      <name>timestamp</name>
      <anchorfile>structTempSensorDHT11DeviceType_1_1SensorReading.html</anchorfile>
      <anchor>ae5151badd21b613deebaf9fdce247ab1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ServiceCallData</name>
    <filename>structServiceCallData.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>deviceOrRoomId</name>
      <anchorfile>structServiceCallData.html</anchorfile>
      <anchor>ae952fb48741c88bf031145a4ce919e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ServiceParameters_set1</type>
      <name>parameters</name>
      <anchorfile>structServiceCallData.html</anchorfile>
      <anchor>a331deee25f460ad30c05b7e935c8f87a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DeviceServicesType</type>
      <name>serviceType</name>
      <anchorfile>structServiceCallData.html</anchorfile>
      <anchor>a16c9917f66937126b44865d9a9af008c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RemoteDevicesManager::ServiceCallFingerprint</name>
    <filename>structRemoteDevicesManager_1_1ServiceCallFingerprint.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>structRemoteDevicesManager_1_1ServiceCallFingerprint.html</anchorfile>
      <anchor>a24cacf91f0ee8381a0ab0046b5f63ef9</anchor>
      <arglist>(ServiceCallFingerprint &amp;other)</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>structRemoteDevicesManager_1_1ServiceCallFingerprint.html</anchorfile>
      <anchor>acbf5b2d949d516c2e86abb2e49a39050</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>memoryRequestDirection</name>
      <anchorfile>structRemoteDevicesManager_1_1ServiceCallFingerprint.html</anchorfile>
      <anchor>afc86587f7e661212992b8f84121fd655</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ServiceOverloadingFunction</type>
      <name>overloading</name>
      <anchorfile>structRemoteDevicesManager_1_1ServiceCallFingerprint.html</anchorfile>
      <anchor>abdd1beb500cb79cba5113e64cc3f44d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DeviceServicesType</type>
      <name>serviceName</name>
      <anchorfile>structRemoteDevicesManager_1_1ServiceCallFingerprint.html</anchorfile>
      <anchor>adf312e013059150a4c47286d4533b79b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ServiceInformation</name>
    <filename>structServiceInformation.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>coreTemperature</name>
      <anchorfile>structServiceInformation.html</anchorfile>
      <anchor>af57f91979a6ba4648cfc384980ebb9ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>ramFree</name>
      <anchorfile>structServiceInformation.html</anchorfile>
      <anchor>a9504927f54ada70991cf3e82bc58c19b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>ramTotal</name>
      <anchorfile>structServiceInformation.html</anchorfile>
      <anchor>a549b9ce65eaa42fa81f4ed7e7a294314</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>ramUsed</name>
      <anchorfile>structServiceInformation.html</anchorfile>
      <anchor>afae1ab69da020c2f97e229f6b4bf7eb0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ServiceParameters_set1</name>
    <filename>structServiceParameters__set1.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>a</name>
      <anchorfile>structServiceParameters__set1.html</anchorfile>
      <anchor>a7f5378263c5f9955000c5b2869f04582</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>b</name>
      <anchorfile>structServiceParameters__set1.html</anchorfile>
      <anchor>a4642bc0c6ba2be8ba86f870046de8076</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>c</name>
      <anchorfile>structServiceParameters__set1.html</anchorfile>
      <anchor>ab1d4b84f6dcadb2857dd79bbf217e846</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>d</name>
      <anchorfile>structServiceParameters__set1.html</anchorfile>
      <anchor>af0f81fdcc487b9e8d41de605f907e167</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>e</name>
      <anchorfile>structServiceParameters__set1.html</anchorfile>
      <anchor>a1997c679152ead6319c6b8efd9305bf2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ServiceParameters_set2</name>
    <filename>structServiceParameters__set2.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>a</name>
      <anchorfile>structServiceParameters__set2.html</anchorfile>
      <anchor>a57814b892fcbcf8e2665cb7ce6357d4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>b</name>
      <anchorfile>structServiceParameters__set2.html</anchorfile>
      <anchor>aec82ac14cdfb9fa897c4e022e94e76ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>c</name>
      <anchorfile>structServiceParameters__set2.html</anchorfile>
      <anchor>a13c12ef08b5b5e4a6c8503e73c6f7622</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>d</name>
      <anchorfile>structServiceParameters__set2.html</anchorfile>
      <anchor>a183c1349854511402e39c902c806917e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>e</name>
      <anchorfile>structServiceParameters__set2.html</anchorfile>
      <anchor>ad6c0b0cc11a2f7a78d2718e205e71168</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ServiceParameters_set3</name>
    <filename>structServiceParameters__set3.html</filename>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>structServiceParameters__set3.html</anchorfile>
      <anchor>aa1819e59eb446ffd8b0a123d8badc8c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>additionalParam</name>
      <anchorfile>structServiceParameters__set3.html</anchorfile>
      <anchor>a1aa45648f2a1c272c1f385072f513e71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>buff</name>
      <anchorfile>structServiceParameters__set3.html</anchorfile>
      <anchor>a0bf3a355035a2b5a4e68b63c08965923</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>direction</name>
      <anchorfile>structServiceParameters__set3.html</anchorfile>
      <anchor>a0d9c121e4c449d0b78e693d61fb8ac71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>size</name>
      <anchorfile>structServiceParameters__set3.html</anchorfile>
      <anchor>a6536715168bb09e83a635dd7bb5d7330</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SingleWaveAnimation</name>
    <filename>classSingleWaveAnimation.html</filename>
    <base>ILiveAnimation</base>
    <member kind="enumeration">
      <type></type>
      <name>Direction</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>af1ed56fbffac36f9ce9e5060c9ce40ac</anchor>
      <arglist></arglist>
      <enumvalue file="classSingleWaveAnimation.html" anchor="af1ed56fbffac36f9ce9e5060c9ce40acad1e80c506f4c89e3c4fb1c1b4867a03c">LeftToRight</enumvalue>
      <enumvalue file="classSingleWaveAnimation.html" anchor="af1ed56fbffac36f9ce9e5060c9ce40acab7b0ea3028791689ea070674776855e2">RightToLeft</enumvalue>
    </member>
    <member kind="function">
      <type></type>
      <name>SingleWaveAnimation</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a0a826ede3384cd055e93124dc7b0dcd2</anchor>
      <arglist>(uint16_t width, Direction dir, float dimFactor=0.3f)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SingleWaveAnimation</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a6e00f1e7c402edc69f133bb6944c934b</anchor>
      <arglist>() override=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a284dd309f7337c8bce4038cc4e632d77</anchor>
      <arglist>(LedColor *stripPtr, uint16_t count) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>onStart</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>acf724af1ddda6fb12933bb631a111bb5</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a7a30cd81dc68c7673f361eded9e07237</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>onStop</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>aa08a7b66473d9ed71ac23c556e4de5b8</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>advance</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>ac407cfee64b6988938c2a61355278327</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>computeAverageColor</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>af891aa944e0851010e4138df107f2a29</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor</type>
      <name>averageColor</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a5765140c82c07ef241d16cf9455b44e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>dimFactor</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a0788b213436e68bc181c5e40365a1779</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Direction</type>
      <name>dir</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>a90fb6eff5f6a2887f5edf9932a669a73</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pos</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>abb7bb7f61774c71328708ba033a48c33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>width</name>
      <anchorfile>classSingleWaveAnimation.html</anchorfile>
      <anchor>ac4d8353b79c64cd06a575638f6d309c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SmoothWaveAnimation</name>
    <filename>classSmoothWaveAnimation.html</filename>
    <base>ILiveAnimation</base>
    <member kind="enumeration">
      <type></type>
      <name>Direction</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a53141c8461b3133809e1928966eeec1e</anchor>
      <arglist></arglist>
      <enumvalue file="classSmoothWaveAnimation.html" anchor="a53141c8461b3133809e1928966eeec1ead1e80c506f4c89e3c4fb1c1b4867a03c">LeftToRight</enumvalue>
      <enumvalue file="classSmoothWaveAnimation.html" anchor="a53141c8461b3133809e1928966eeec1eab7b0ea3028791689ea070674776855e2">RightToLeft</enumvalue>
    </member>
    <member kind="function">
      <type></type>
      <name>SmoothWaveAnimation</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a2653e88c2f155aedde0e0b982e0a72a6</anchor>
      <arglist>(uint16_t width, Direction dir, float dimFactor=0.3f)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SmoothWaveAnimation</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a33050e6df23283bbd601e3436029cbfa</anchor>
      <arglist>() override=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a8356fbbd1fabc7b35dd7836eb9f1cce0</anchor>
      <arglist>(LedColor *stripPtr, uint16_t count) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>onStart</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>aadf9d90cd4bf5035cb8d0004bbe9ec5f</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>aa4ffe0a9fee23744f4c39142ddfb4e3d</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>onStop</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a3583bcfaa9b39b480e9a85ffec568d7a</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>advance</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>ac7d23b97df4ca13fe234100dec347d07</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>computeAverageColor</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a073f3c64db07c964bed1fd8ace0a7ee4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor</type>
      <name>averageColor</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a7fbbfddd42536f0c51ef7f86635b3c89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>dimFactor</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>ae0f5cf7d100f848af013118cb07f0d7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Direction</type>
      <name>dir</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a84b8bcdd667ff03fa510383f6a7e588f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pos</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a06c9ec4af894cfb94327a4e1921fc542</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>width</name>
      <anchorfile>classSmoothWaveAnimation.html</anchorfile>
      <anchor>a32600a51aaa7e33ff8a86c2820f27b04</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SparkleInAnimation</name>
    <filename>classSparkleInAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>SparkleInAnimation</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>ade7cd2d5d135d1f4e8f668a319a770b9</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount, Direction dir=Direction::LeftToRight, uint8_t sparkleCount=5)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SparkleInAnimation</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a3fd1b7493087cb573dd0dbe441cd4377</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a5638f69666b0b2c374544ed20193fdd4</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isInProgress</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a3045f52b12d6556e7039c41d4986b3da</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>aec774415b233bd42f9ae289f91a67ede</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreColors</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a155a06a655cc59007123034d888c20c1</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>acd01cd8f0d454b867e2ff8213eb45a2e</anchor>
      <arglist>(bool) override</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static uint32_t</type>
      <name>randR</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>ab3e53fc53839dc65de3cbb1a33566bd5</anchor>
      <arglist>(uint32_t *state)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>animationBuffer</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a4f325928b7e3150f9493665eb82309bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Direction</type>
      <name>direction</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a688e084198abcfba41657bffd9670b46</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>afd7962140e347c4b5373e1e69cd8b36d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a5415ce853efb9d90b1f4d750334d3167</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>position</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a1f3d4e6b1c912d2958e86db3580a4081</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>rngState</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a8b803a0a491f4f68ec668fb5bf9c1d9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>sparkleCount</name>
      <anchorfile>classSparkleInAnimation.html</anchorfile>
      <anchor>a3c57220726f136bf4fd0bd02cf1b6365</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CyclicProfiler::Stat</name>
    <filename>structCyclicProfiler_1_1Stat.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>backlog</name>
      <anchorfile>structCyclicProfiler_1_1Stat.html</anchorfile>
      <anchor>a47b4565d8c34686a129acc3af645043e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>count</name>
      <anchorfile>structCyclicProfiler_1_1Stat.html</anchorfile>
      <anchor>acd94c6b8679cc25a55a412464360c54f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>maxTime</name>
      <anchorfile>structCyclicProfiler_1_1Stat.html</anchorfile>
      <anchor>afbe584177b790d2204c3f3a14d79005b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>totalTime</name>
      <anchorfile>structCyclicProfiler_1_1Stat.html</anchorfile>
      <anchor>a0d16835a8bc8886b12de0015fd943fda</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SystemErrorType</name>
    <filename>structSystemErrorType.html</filename>
    <member kind="variable">
      <type>String</type>
      <name>comment</name>
      <anchorfile>structSystemErrorType.html</anchorfile>
      <anchor>a22de54cfe7269989ac67ddc79cbbe04a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>lastOccurrenceTime</name>
      <anchorfile>structSystemErrorType.html</anchorfile>
      <anchor>aa3f1cf57597075af71b227cecf0fe2e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>occurrenceCount</name>
      <anchorfile>structSystemErrorType.html</anchorfile>
      <anchor>a0c1b28f575f68f8ec791ff87dead1fa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>timeOfOccurrence</name>
      <anchorfile>structSystemErrorType.html</anchorfile>
      <anchor>ad60df0c003f40600427ebf83e5f63561</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TempSensorDHT11DeviceType</name>
    <filename>classTempSensorDHT11DeviceType.html</filename>
    <base>Device</base>
    <class kind="struct">TempSensorDHT11DeviceType::SensorReading</class>
    <member kind="function">
      <type></type>
      <name>TempSensorDHT11DeviceType</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>ad685edc53faa5c345c8e92de66b64e7c</anchor>
      <arglist>(DeviceConfigSlotType nvmData, std::function&lt; RtcTime(void)&gt; getTimeCallback)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a98f99fbdafa6a41a2644195dc5da98c6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aa7db8356d84458a66b1c5ef13a6b8e06</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a022c07180d40d60f54e8f5b58624203f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a9c3dc56b9001fa5ee05579737eb10274</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>ab32b340d0aadbdaf26307ba30c5e7833</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>ad83c7552975edd13499562c615057f51</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aca794a049493aca2d15d8a3e25967693</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a4e81bfe2181462a972f149012f08606e</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>ad9d3e7e0c36b05bdf1510109c0926bfd</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a62c65fa482f44032b8737a7cd6959dc1</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual void</type>
      <name>dhtSensorRecords</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aa1da565ad73ecf1840e48683d1d9358a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual void</type>
      <name>printSensorData</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a8de1acdca192270141da51d4b5c456f3</anchor>
      <arglist>(float temp, float humid, SensorReading reading)</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual void</type>
      <name>temHumReading</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>ac92a0cbf686e80850a511c8994c1c125</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>currentHumid</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a33d1e33b8ba0e15c827ed3190eef6066</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>currentTemp</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a1be87a95be7db73c1e69295b597248a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>deviceId</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a16a4901b2164db9045a55f034ce97eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>String</type>
      <name>deviceName</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>af524389652b403f543b45cbe965946cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>DHT *</type>
      <name>dht</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>abcaaa795e499f62714cdd90a6507512a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; RtcTime(void)&gt;</type>
      <name>getTime</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a2c33d2f07aec49ef94043f594d7223bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>isOn</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aaa034f01e99475b8db8150cf14b2e09e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>lastCheckedTime</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a8676a73296133c102aa4f6d587433a8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>lastDataUpdateTime</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a0a436b3623fe16891d50e4a5f48ca310</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>lastLoggedMinute</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a875791e729cef9306da0fbf0703ba479</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned long</type>
      <name>lastStoredTime</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aa9f9e75d6d34bd367298bab274f5f0a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>lastTemp</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a9f8cc6d051b6010cc03c1a110dd8ce2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const size_t</type>
      <name>MAX_ENTRIES</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a9bda517e2c15a270d84e05d2d7f597ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pinNumber</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>afa987d58b8a82063a6a87f2c23f8b486</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>roomId</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a61e58c796ee2a59f55019f66a177785e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; SensorReading &gt;</type>
      <name>sensorData</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>a700a135fabe149da709b2e14070bede3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>temHumSensError</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aac609aeeb4977042d283db907fed4bea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const unsigned long</type>
      <name>TIME_STORE_PERIOD</name>
      <anchorfile>classTempSensorDHT11DeviceType.html</anchorfile>
      <anchor>aeb49908c5299ba7ea9315516b6781a4b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TestDeviceType</name>
    <filename>classTestDeviceType.html</filename>
    <base>Device</base>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>cyclic</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>a666849b40b7cf080527b221a0510c800</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DeviceDescription</type>
      <name>getDeviceDescription</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>ac84831b86617f683410046896fb5e3ea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceIdentifier</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>a96030d5918a400614493f30e7ab1324f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint8_t</type>
      <name>getDeviceType</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>aae0f46e18084f502ce40476491f01929</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual uint16_t</type>
      <name>getExtendedMemoryLength</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>a7f35176b4c2d5dda4b3c5bc2c972f4b4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>a608bed92471180489d146301e7556139</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>abfd2aa3d25ca5939eac4ef7059b38a28</anchor>
      <arglist>(DeviceServicesType serviceType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>a32be708ba08c5e369deeaeb1d4e6dabb</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set1 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>af04a2d9579006d78344b330eaec937e3</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set2 param)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ServiceRequestErrorCode</type>
      <name>service</name>
      <anchorfile>classTestDeviceType.html</anchorfile>
      <anchor>ae85a0051d9f8a96525c7b618db94cb60</anchor>
      <arglist>(DeviceServicesType serviceType, ServiceParameters_set3 param)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TimeMaster</name>
    <filename>classTimeMaster.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cyclic</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>afc9b64c74c0aa0ab798f68f9ff5ec2b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deinit</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>aab7a075b0e4fcf201afab3af0c42341c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned long</type>
      <name>getEpochTime</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a573ad003314f4a094eb88877c205ae62</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static RtcTime</type>
      <name>getRtcTime</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a7120552e5e1b47caca89cd06c8a07764</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a806fe45aa5bf87bb0f2ed1e9206757ae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setTimeZone</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a0bfec395595cc63b554553ca35a40ca8</anchor>
      <arglist>(int timeZoneOffset)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateNtpVariables</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>aa0c36e25cb092bad234b7eea2e741bf1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>initialRetryCount</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a94af886c8a09e17230c50ca12ed238f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>lastMillis</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a8216b237371acd87e7b92c45e79c0c52</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>lastNTPTime</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a623e567a5a5a462d4223898f834f29d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>lastUpdateTime</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a2c408fdcaa5f56ae1fa219ee70537e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>ntpAvailable</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>adf5b50d68e783cb7d0c9ff0703bf7e9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static WiFiUDP</type>
      <name>ntpUDP</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>ac79aaf9f91e99b52d06da10d1e21fa0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>startupTimeInitialized</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>aa89c59203c79e80f63d55a125bbfbf1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static NTPClient</type>
      <name>timeClient</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>ada75b680ea71b3ea8cc358419cd939c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static uint8_t</type>
      <name>timeSyncFailureCount</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a594dda5cd89de87a5b63760f0a31d425</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static unsigned long</type>
      <name>updateInterval</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>a4305daeb89d344b710ad77b5ade9ce63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>wasNtpEverSynced</name>
      <anchorfile>classTimeMaster.html</anchorfile>
      <anchor>aaba46593d107e2df11ba48ef4a9dec26</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Transaction</name>
    <filename>structTransaction.html</filename>
    <member kind="variable">
      <type>RcRequest</type>
      <name>request</name>
      <anchorfile>structTransaction.html</anchorfile>
      <anchor>ad34e5c75786da907540d49e401de36dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::optional&lt; RcResponse &gt;</type>
      <name>response</name>
      <anchorfile>structTransaction.html</anchorfile>
      <anchor>aabade81845a6c9d17340370c22523416</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TwinkleInAnimation</name>
    <filename>classTwinkleInAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>TwinkleInAnimation</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a382bb82ccde4b6fb8d458d21865400e2</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TwinkleInAnimation</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a78c192f01d8379a20ab4d712cbeb796b</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a279e69bd57cee27fb93d4940c7c23873</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isInProgress</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a7a7b64ba91ef585e7cb3101d954f9ac7</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>aef2bbb067c9db7413df7c950fce09076</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreColors</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>ae55a3cbe1cb3db16dd2ea27f275511c7</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a06470af3c60bacaae3fc9f4327ea56d4</anchor>
      <arglist>(bool) override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>animationBuffer</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>aa5aa0bda056d5a198181c018e4fd23c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a6f79cf0c6dbc4625eb2a4adbd7454278</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>ledsCount</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>aff106b3cb4a58ec20c8ac90e5069d75b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>a6060a0be65512f023000f668125a48d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; bool &gt;</type>
      <name>remaining</name>
      <anchorfile>classTwinkleInAnimation.html</anchorfile>
      <anchor>af44e37f4f6d139ae9b8453cc9774a688</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>UDPAdapter</name>
    <filename>classUDPAdapter.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a0e05820ab1066fc9ed422400d23094e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>registerOnUdpReceiveCallback</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a2e3a79d42cb5f2746fd7da9c9a76e1ce</anchor>
      <arglist>(std::function&lt; void(MessageUDP &amp;)&gt; callback)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>send</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>aecd129cc8a41f927fd958acb22c6a63d</anchor>
      <arglist>(MessageUDP &amp;msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>sendingAllowed</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a833b046673259f0424526b5e0e4ca4f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a64878880e301e861adcc9251ad8eb83f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char *</type>
      <name>broadcastIP</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a6c901725cd612c9ec1b21a11374ff2a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static long</type>
      <name>lastSendTime</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a05b0b4f97357e129ff06598590c65f9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::function&lt; void(MessageUDP &amp;)&gt;</type>
      <name>onReceiveCallback</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a9d4ba8fca1e8259682e4301cd5c118e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned int</type>
      <name>senderPort</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>ac31d1eef34cc2119a1e7ce0fe8aef666</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static WiFiUDP</type>
      <name>udp</name>
      <anchorfile>classUDPAdapter.html</anchorfile>
      <anchor>a158cfbb56282c0176c6c72f3e016ac0e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>UINotificationsControlAPI</name>
    <filename>structUINotificationsControlAPI.html</filename>
    <member kind="variable">
      <type>std::function&lt; bool(UserInterfaceNotification &amp;)&gt;</type>
      <name>createNotification</name>
      <anchorfile>structUINotificationsControlAPI.html</anchorfile>
      <anchor>a0198b464c85f32e3307cc773cb54d1e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; uint8_t(void)&gt;</type>
      <name>getActiveNotificationsCount</name>
      <anchorfile>structUINotificationsControlAPI.html</anchorfile>
      <anchor>a192510d622ccba1b02a4c90be4365613</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; UserInterfaceNotification(void)&gt;</type>
      <name>getOldestNotification</name>
      <anchorfile>structUINotificationsControlAPI.html</anchorfile>
      <anchor>ae46dedd4791e9d22be0261499feb20d5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>UserInterfaceNotification</name>
    <filename>structUserInterfaceNotification.html</filename>
    <member kind="enumvalue">
      <name>ERROR</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498ba5d9261f84331cd7ba0e0dfa8801a5f75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INFO</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498babae4c1da4dda89b3b1a1cf2516dba06a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>Type</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INFO</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498babae4c1da4dda89b3b1a1cf2516dba06a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WARNING</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498ba55dfb7164caea74fc50892ff48ab285a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERROR</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498ba5d9261f84331cd7ba0e0dfa8801a5f75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WARNING</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a064918e3728ce4cb49aabb423500498ba55dfb7164caea74fc50892ff48ab285a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>toJson</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a36c9af047240f85cd5ae3b767f96b576</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>body</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>abeb2560896a15a48c798e20275d05bdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>time</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a19a8a3fa3740d835f80446f4634d32dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>title</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a7bb0f6ce0592945e3ca8b28c49f515b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Type</type>
      <name>type</name>
      <anchorfile>structUserInterfaceNotification.html</anchorfile>
      <anchor>a170dcc2f79e326ec0e64bd4bc46924ef</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WaveInAnimation</name>
    <filename>classWaveInAnimation.html</filename>
    <base>ILedAnimation</base>
    <member kind="function">
      <type></type>
      <name>WaveInAnimation</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>ab04c4d8a8fb05ce619ae804eaebb801f</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount, float wavelength=10.0f, float speed=0.5f)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WaveInAnimation</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a41e4d742f5bf11f9ff9e53879dc5f173</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a5c033e7cdb491aa26d7a65b2687444b7</anchor>
      <arglist>(LedColor *mainStripPtr, uint16_t ledsCount) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isInProgress</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a37c7eb40ba1741a8994f94e882d652e7</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>afbea94a8d43414ce67f230871a6b413a</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreColors</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>af8f69a7ea01fe49c9894a74c7d2bed15</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a04c26bd8a29da2129420d57a3e7819c4</anchor>
      <arglist>(bool) override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>animationBuffer</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a50b4afce3affa8e470964f7ed7afb741</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>inProgress</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>af378056b61d21e666093b2021ed4203a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>ledsCount</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a9aa529d79500dd8dcef7fd942193bfdb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LedColor *</type>
      <name>mainStrip</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a60f80393a2e360f532569c2f625ae048</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>speed</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>aeb3f12417483fc3c265783f279fd4fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>time</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>a8419cfa9d96c28055fbf26c5cd1ccb12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>wavelength</name>
      <anchorfile>classWaveInAnimation.html</anchorfile>
      <anchor>af1a804f7d07eb24440abf476bf2bd77c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WiFiAdapter</name>
    <filename>classWiFiAdapter.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>connectToNetwork</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a81efe2728b837179b851239aace90ee9</anchor>
      <arglist>(const String ssid, const String password, bool activeWait)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>createAccessPoint</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a0e923990a3abe1cdaec0ac6321de893e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>disconnect</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a4eba15a5dd06533016a67cf320dac329</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>enableMDNSResponder</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a8369386f2781da0a6850636f571ba7c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>getIpString</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>af9090fcff3f604990cce3fe2f4e9c39e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetworkStatus</type>
      <name>getNetworkStatus</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a1403b76f008cd8f455f3bb6d52e33303</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a1866e51ffcef3af32df35f6cada499c8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isConnected</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>aba4ab3834039a4ab0f94ce0bbbc79261</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reconnect</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>ad785bb8d034b32371be630a8ae2c34ae</anchor>
      <arglist>(String &amp;ssid, String &amp;pwd)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>task</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a6a961ba9a9a2813330ca45584396e352</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>handleReconnection</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a221370670363eabe94cbe4c4bf5a81d6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>manualStatusCheck</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>abbeb36a1aad320ea055d2febc50b6de9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>updateDataContainerIpAddress</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a886b83f3ebb60a4d135f968127f5ac87</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>waitForConnection500ms</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a267e2444ebd9b4a8c96ac53bdefd0720</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>WiFiEvent</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>ada433ffbbbd42383a6d14e1866a506ca</anchor>
      <arglist>(WiFiEvent_t event)</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>connectionInitialized</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a363d8332668aa5425403600a196815ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static NetworkStatus</type>
      <name>currentStatus</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>adf632a1efe3b6bbca94e9eb9f6425f20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>isConnectedFlag</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a0d832d85710ab3e5c20653842a0e3bc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static String</type>
      <name>mPassword</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a4be20fc9bf561eb9a97e25c709632e35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static String</type>
      <name>mSsid</name>
      <anchorfile>classWiFiAdapter.html</anchorfile>
      <anchor>a5eab29dea5f1f3043f434d32d49beb7b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>DigitalEvent</name>
    <filename>namespaceDigitalEvent.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>ActionType</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>aa23c7a72a1fac5c90c90552dce62c170</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ON</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>aa23c7a72a1fac5c90c90552dce62c170a4c70268a9468353da108b11ffe135980</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OFF</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>aa23c7a72a1fac5c90c90552dce62c170a2b903c410ef8147354d4bc909fb85604</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TOGGLE</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>aa23c7a72a1fac5c90c90552dce62c170a850d3cc071850f0910ea73abf5d95baa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>AffectedType</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>a0bb3a4156d3da60a8de188ad666797d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ROOM</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>a0bb3a4156d3da60a8de188ad666797d0a62c49e46befbccd092c63cf31015113e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEVICE</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>a0bb3a4156d3da60a8de188ad666797d0a2a392ba76123bb1863423602d70d587b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>struct</type>
      <name>__attribute__</name>
      <anchorfile>namespaceDigitalEvent.html</anchorfile>
      <anchor>ad53a4fbe154c87bc52471aed6488c316</anchor>
      <arglist>((packed)) Event</arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>include/devices/LedStrip/Animations</name>
    <path>include/devices/LedStrip/Animations/</path>
    <filename>dir_86fdb9f417200303ee562fa435f84fc1.html</filename>
    <dir>include/devices/LedStrip/Animations/live</dir>
    <file>BounceInAnimation.hpp</file>
    <file>FadeIn.hpp</file>
    <file>FadeOut.hpp</file>
    <file>ILedAnimation.hpp</file>
    <file>ILiveAnimation.hpp</file>
    <file>RollInAnimation.hpp</file>
    <file>RollOutAnimation.hpp</file>
    <file>SparkleInAnimation.hpp</file>
    <file>TwinkleInAnimation.hpp</file>
    <file>WaveInAnimation.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/device/LedStrip/Animations</name>
    <path>src/device/LedStrip/Animations/</path>
    <filename>dir_de9527c8bc0a1d603f929a4ef88d515f.html</filename>
    <dir>src/device/LedStrip/Animations/live</dir>
    <file>BounceInAnimation.cpp</file>
    <file>FadeIn.cpp</file>
    <file>FadeOut.cpp</file>
    <file>ILedAnimation.cpp</file>
    <file>ILiveAnimation.cpp</file>
    <file>RollInAnimation.cpp</file>
    <file>RollOutAnimation.cpp</file>
    <file>SparkleInAnimation.cpp</file>
    <file>TwinkleInAnimation.cpp</file>
    <file>WaveInAnimation.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app</name>
    <path>include/os/app/</path>
    <filename>dir_8516fa42a63f32e5afbde9ae9ba81abc.html</filename>
    <dir>include/os/app/config</dir>
    <dir>include/os/app/DigitalEvent</dir>
    <dir>include/os/app/display</dir>
    <dir>include/os/app/http</dir>
    <dir>include/os/app/remoteControl</dir>
    <file>devicemanager.hpp</file>
    <file>deviceProvider.hpp</file>
    <file>NotificationHandler.hpp</file>
    <file>RemoteDevicesManager.hpp</file>
    <file>SafeResetManager.hpp</file>
    <file>timeMaster.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app</name>
    <path>src/os/app/</path>
    <filename>dir_d8879d3950f606b1eb55bc4801344107.html</filename>
    <dir>src/os/app/config</dir>
    <dir>src/os/app/DigitalEvent</dir>
    <dir>src/os/app/display</dir>
    <dir>src/os/app/http</dir>
    <dir>src/os/app/HwDependency</dir>
    <dir>src/os/app/remoteControl</dir>
    <file>devicemanager.cpp</file>
    <file>deviceProvider.cpp</file>
    <file>NotificationHandler.cpp</file>
    <file>RemoteDevicesManager.cpp</file>
    <file>SafeResetManager.cpp</file>
    <file>timeMaster.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app/config</name>
    <path>include/os/app/config/</path>
    <filename>dir_856736177dfdd1c4cddf0022547f7f99.html</filename>
    <file>configprovider.hpp</file>
    <file>ExtendedMemoryManager.hpp</file>
    <file>persistentMemoryAccess.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/config</name>
    <path>src/os/app/config/</path>
    <filename>dir_fa83736bba2c01e70c8d48cbfddc7b9f.html</filename>
    <file>configprovider.cpp</file>
    <file>ExtendedMemoryManager.cpp</file>
    <file>persistentMemoryAccess.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/devices/CustomHardware</name>
    <path>include/devices/CustomHardware/</path>
    <filename>dir_2722d521f340028fe918c760286a3ac9.html</filename>
    <file>HwButton.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/device/CustomHardware</name>
    <path>src/device/CustomHardware/</path>
    <filename>dir_e594bf0220011c7486e1070e71180f23.html</filename>
    <file>HwButton.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/datacontainer</name>
    <path>include/os/datacontainer/</path>
    <filename>dir_354f5560a0007a3f15f6fe1199dd7cda.html</filename>
    <file>datacontainer.hpp</file>
    <file>datacontainertypes.hpp</file>
    <file>DeviceControlApi.hpp</file>
    <file>NvmConfigSlotDefinition.hpp</file>
    <file>sigmessages.hpp</file>
    <file>signals.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/datacontainer</name>
    <path>src/os/datacontainer/</path>
    <filename>dir_65ccf2ea6218bdcef20f8744d334eec8.html</filename>
    <file>datacontainer.cpp</file>
  </compound>
  <compound kind="dir">
    <name>src/device</name>
    <path>src/device/</path>
    <filename>dir_16dc92632bab467dc50b01bd02b3341a.html</filename>
    <dir>src/device/CustomHardware</dir>
    <dir>src/device/LedStrip</dir>
    <file>distanceSensor.cpp</file>
    <file>onoffdevice.cpp</file>
    <file>tempSensorDHT11.cpp</file>
    <file>TestDeviceType.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/devices</name>
    <path>include/devices/</path>
    <filename>dir_0ae5fd04a9b991f2c376fafa197d3147.html</filename>
    <dir>include/devices/CustomHardware</dir>
    <dir>include/devices/LedStrip</dir>
    <file>AdvancedControls.hpp</file>
    <file>device.hpp</file>
    <file>distanceSensor.hpp</file>
    <file>onoffdevice.hpp</file>
    <file>tempSensorDHT11.hpp</file>
    <file>TestDeviceType.hpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app/DigitalEvent</name>
    <path>include/os/app/DigitalEvent/</path>
    <filename>dir_25b8542419db09ca135b70847ef4665d.html</filename>
    <file>DigitalEventDefinitions.hpp</file>
    <file>DigitalEventReceiver.hpp</file>
    <file>DigitalEventTransmitter.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/DigitalEvent</name>
    <path>src/os/app/DigitalEvent/</path>
    <filename>dir_d78338ef000a26d0483f7ab9099a27f0.html</filename>
    <file>DigitalEventReceiver.cpp</file>
    <file>DigitalEventTransmitter.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app/display</name>
    <path>include/os/app/display/</path>
    <filename>dir_147203a347b5358bf15feb2a49133243.html</filename>
    <file>DeviceDescriptionPacker.hpp</file>
    <file>Display.hpp</file>
    <file>DisplayDatatypes.hpp</file>
    <file>DisplayServer.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/display</name>
    <path>src/os/app/display/</path>
    <filename>dir_f4afa7644a0e1c2f8e61b41bd1651951.html</filename>
    <file>DeviceDescriptionPacker.cpp</file>
    <file>Display.cpp</file>
    <file>DisplayServer.cpp</file>
  </compound>
  <compound kind="dir">
    <name>docs</name>
    <path>docs/</path>
    <filename>dir_49e56c817e5e54854c35e136979f97ca.html</filename>
  </compound>
  <compound kind="dir">
    <name>include/os/drivers</name>
    <path>include/os/drivers/</path>
    <filename>dir_caaf8b51d605be9810e5b10d09a5fee4.html</filename>
    <file>networkdriver.hpp</file>
    <file>ota.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/drivers</name>
    <path>src/os/drivers/</path>
    <filename>dir_f29d85dd4a02f94d9087189ebca38efc.html</filename>
    <file>networkdriver.cpp</file>
    <file>ota.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app/http</name>
    <path>include/os/app/http/</path>
    <filename>dir_12ffaae0b1b3d68d4559f0d8d67996d3.html</filename>
    <dir>include/os/app/http/tempGauge</dir>
    <file>AdvancedControlsLoader.hpp</file>
    <file>configPageHttp.h</file>
    <file>deviceWidgets.h</file>
    <file>HttpAsyncRequestHandler.hpp</file>
    <file>httpserver.hpp</file>
    <file>javascript.h</file>
    <file>pageHead.h</file>
    <file>renderRoomsJS.h</file>
    <file>style.h</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/http</name>
    <path>src/os/app/http/</path>
    <filename>dir_556aa9bd837eebf911a6370a247dd75e.html</filename>
    <dir>src/os/app/http/requestCallbacks</dir>
    <file>AdvancedControlsLoader.cpp</file>
    <file>HttpAsyncRequestHandler.cpp</file>
    <file>httpserver.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/HwDependency</name>
    <path>include/os/HwDependency/</path>
    <filename>dir_9fd4ea950c48e8017e970f939e91931b.html</filename>
    <file>RFManager.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/HwDependency</name>
    <path>src/os/app/HwDependency/</path>
    <filename>dir_db31c5af7699fd6e5ef87a59585885fd.html</filename>
    <file>RFManager.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include</name>
    <path>include/</path>
    <filename>dir_d44c64559bbebec7f509842c48db8b23.html</filename>
    <dir>include/devices</dir>
    <dir>include/os</dir>
    <file>SystemDefinition.hpp</file>
  </compound>
  <compound kind="dir">
    <name>include/devices/LedStrip</name>
    <path>include/devices/LedStrip/</path>
    <filename>dir_3f9da3614ac9460d4bb93574672ca3ba.html</filename>
    <dir>include/devices/LedStrip/Animations</dir>
    <file>LedWS1228b.hpp</file>
    <file>Segmented_LedWS1228b.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/device/LedStrip</name>
    <path>src/device/LedStrip/</path>
    <filename>dir_712fbb69e00e7a414aba3531a051f5e2.html</filename>
    <dir>src/device/LedStrip/Animations</dir>
    <file>LedWS1228b.cpp</file>
    <file>Segmented_LedWS1228b.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/devices/LedStrip/Animations/live</name>
    <path>include/devices/LedStrip/Animations/live/</path>
    <filename>dir_5657a69ee96261376c85258213edad3e.html</filename>
    <file>SingleWaveAnimation copy.hpp</file>
    <file>SingleWaveAnimation.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/device/LedStrip/Animations/live</name>
    <path>src/device/LedStrip/Animations/live/</path>
    <filename>dir_b55b21b3b65f0e65e355c1b0fa40bce0.html</filename>
    <file>SingleWaveAnimation.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os</name>
    <path>include/os/</path>
    <filename>dir_7b371d9fdb0359ac564866f88df60f77.html</filename>
    <dir>include/os/app</dir>
    <dir>include/os/datacontainer</dir>
    <dir>include/os/drivers</dir>
    <dir>include/os/HwDependency</dir>
    <dir>include/os/tools</dir>
    <file>CyclicProfiler.hpp</file>
    <file>HomeStation_os.hpp</file>
    <file>Logger.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os</name>
    <path>src/os/</path>
    <filename>dir_a71443761d886d6152d96ebd3fda63af.html</filename>
    <dir>src/os/app</dir>
    <dir>src/os/datacontainer</dir>
    <dir>src/os/drivers</dir>
    <dir>src/os/tools</dir>
    <file>CyclicProfiler.cpp</file>
    <file>HomeStation_os.cpp</file>
    <file>Logger.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app/remoteControl</name>
    <path>include/os/app/remoteControl/</path>
    <filename>dir_f5768763ecf096468a4550894237aae1.html</filename>
    <file>rc_dataTypes.hpp</file>
    <file>RC_publicDataTypes.hpp</file>
    <file>rcRequest.hpp</file>
    <file>rcResponse.hpp</file>
    <file>remoteControlClient.hpp</file>
    <file>remotecontrolserver.hpp</file>
    <file>ReqestProcessor.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/remoteControl</name>
    <path>src/os/app/remoteControl/</path>
    <filename>dir_f491c33c302c297fb588c62fb78bcf64.html</filename>
    <file>rcRequest.cpp</file>
    <file>rcResponse.cpp</file>
    <file>remoteControlClient.cpp</file>
    <file>remotecontrolserver.cpp</file>
    <file>RequestProcessor.cpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/app/http/requestCallbacks</name>
    <path>src/os/app/http/requestCallbacks/</path>
    <filename>dir_ed503ca3b75bccf09e43eed8c50f0f22.html</filename>
    <file>asyncHandlers.cpp</file>
    <file>constantHandlers.cpp</file>
    <file>parameterizedHandlers.cpp</file>
  </compound>
  <compound kind="dir">
    <name>src</name>
    <path>src/</path>
    <filename>dir_68267d1309a1af8e8297ef4c3efbcdba.html</filename>
    <dir>src/device</dir>
    <dir>src/os</dir>
    <file>build_info.h</file>
    <file>main.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/os/app/http/tempGauge</name>
    <path>include/os/app/http/tempGauge/</path>
    <filename>dir_576c0a41a4168b27e62fd3a8a0abf3b0.html</filename>
    <file>tempGaugeCSS.h</file>
    <file>tempGaugeJS.h</file>
  </compound>
  <compound kind="dir">
    <name>include/os/tools</name>
    <path>include/os/tools/</path>
    <filename>dir_893dc215ea1423cb30b4e3506d7a249c.html</filename>
    <file>ExtendedDataAllocator.hpp</file>
    <file>ExtendedDataTypes.hpp</file>
    <file>messageUDP.hpp</file>
    <file>udpAdapter.hpp</file>
    <file>wifiAdapter.hpp</file>
  </compound>
  <compound kind="dir">
    <name>src/os/tools</name>
    <path>src/os/tools/</path>
    <filename>dir_05d21ad7d214aa0bba0af8d6e374f127.html</filename>
    <file>ExtendedDataAllocator.cpp</file>
    <file>messageUDP.cpp</file>
    <file>udpAdapter.cpp</file>
    <file>wifiAdapter.cpp</file>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>HomeControlStation</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="HomeControlStation">md_docs_2DoxygenMainPage</docanchor>
  </compound>
</tagfile>
