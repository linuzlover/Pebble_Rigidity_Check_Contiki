<?xml version="1.0" encoding="UTF-8"?>
<simconf>
  <project EXPORT="discard">[CONTIKI_DIR]/tools/cooja/apps/mrm</project>
  <project EXPORT="discard">[CONTIKI_DIR]/tools/cooja/apps/mspsim</project>
  <project EXPORT="discard">[CONTIKI_DIR]/tools/cooja/apps/avrora</project>
  <project EXPORT="discard">[CONTIKI_DIR]/tools/cooja/apps/serial_socket</project>
  <project EXPORT="discard">[CONTIKI_DIR]/tools/cooja/apps/collect-view</project>
  <project EXPORT="discard">[CONTIKI_DIR]/tools/cooja/apps/powertracker</project>
  <simulation>
    <title>My simulation</title>
    <randomseed>123456</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      se.sics.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <logoutput>40000</logoutput>
    </events>
    <motetype>
      se.sics.cooja.avrmote.MicaZMoteType
      <identifier>micaz1</identifier>
      <description>MicaZ Mote Type #micaz1</description>
      <source>[CONTIKI_DIR]/examples/Pebble_Rigidity_Check_Contiki/attilio_receive.c</source>
      <commands>make attilio_receive.elf TARGET=micaz</commands>
      <firmware>[CONTIKI_DIR]/examples/Pebble_Rigidity_Check_Contiki/attilio_receive.elf</firmware>
      <moteinterface>se.sics.cooja.interfaces.Position</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaZID</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaZLED</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaZRadio</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaClock</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaSerial</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.MoteAttributes</moteinterface>
    </motetype>
    <motetype>
      se.sics.cooja.avrmote.MicaZMoteType
      <identifier>micaz2</identifier>
      <description>MicaZ Mote Type #micaz2</description>
      <source>[CONTIKI_DIR]/examples/Pebble_Rigidity_Check_Contiki/attilio_send.c</source>
      <commands>make attilio_send.elf TARGET=micaz</commands>
      <firmware>[CONTIKI_DIR]/examples/Pebble_Rigidity_Check_Contiki/attilio_send.elf</firmware>
      <moteinterface>se.sics.cooja.interfaces.Position</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaZID</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaZLED</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaZRadio</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaClock</moteinterface>
      <moteinterface>se.sics.cooja.avrmote.interfaces.MicaSerial</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.MoteAttributes</moteinterface>
    </motetype>
    <mote>
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>45.66198092345738</x>
        <y>58.31717859169691</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.avrmote.interfaces.MicaZID
        <id>1</id>
      </interface_config>
      <motetype_identifier>micaz1</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>43.72802890429688</x>
        <y>89.73822844176995</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.avrmote.interfaces.MicaZID
        <id>2</id>
      </interface_config>
      <motetype_identifier>micaz1</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>67.65219455656545</x>
        <y>77.12440236033889</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.avrmote.interfaces.MicaZID
        <id>3</id>
      </interface_config>
      <motetype_identifier>micaz1</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>55.8223825577331</x>
        <y>74.94746696240618</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.avrmote.interfaces.MicaZID
        <id>4</id>
      </interface_config>
      <motetype_identifier>micaz2</motetype_identifier>
    </mote>
  </simulation>
  <plugin>
    se.sics.cooja.plugins.SimControl
    <width>280</width>
    <z>0</z>
    <height>160</height>
    <location_x>400</location_x>
    <location_y>0</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.Visualizer
    <plugin_config>
      <skin>se.sics.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>se.sics.cooja.plugins.skins.GridVisualizerSkin</skin>
      <skin>se.sics.cooja.plugins.skins.TrafficVisualizerSkin</skin>
      <skin>se.sics.cooja.plugins.skins.UDGMVisualizerSkin</skin>
      <viewport>6.644196609473991 0.0 0.0 6.644196609473991 -178.89488492285034 -265.9657058802831</viewport>
    </plugin_config>
    <width>400</width>
    <z>1</z>
    <height>400</height>
    <location_x>1</location_x>
    <location_y>1</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.LogListener
    <plugin_config>
      <filter />
    </plugin_config>
    <width>840</width>
    <z>4</z>
    <height>240</height>
    <location_x>400</location_x>
    <location_y>160</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.TimeLine
    <plugin_config>
      <mote>0</mote>
      <mote>1</mote>
      <mote>2</mote>
      <mote>3</mote>
      <showRadioRXTX />
      <showRadioHW />
      <showLEDs />
      <split>-1</split>
      <zoomfactor>500.0</zoomfactor>
    </plugin_config>
    <width>1240</width>
    <z>3</z>
    <height>166</height>
    <location_x>0</location_x>
    <location_y>658</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.Notes
    <plugin_config>
      <notes>Enter notes here</notes>
      <decorations>true</decorations>
    </plugin_config>
    <width>560</width>
    <z>2</z>
    <height>160</height>
    <location_x>680</location_x>
    <location_y>0</location_y>
  </plugin>
</simconf>

