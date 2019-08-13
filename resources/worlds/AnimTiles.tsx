<?xml version="1.0" encoding="UTF-8"?>
<tileset name="AnimTiles" tilewidth="32" tileheight="32" tilecount="32" columns="4">
 <image source="../textures/AnimTiles.png" width="128" height="256"/>
 <tile id="0" type="QuestionBlock">
  <animation>
   <frame tileid="0" duration="500"/>
   <frame tileid="1" duration="500"/>
   <frame tileid="2" duration="500"/>
   <frame tileid="3" duration="500"/>
  </animation>
 </tile>
 <tile id="4" type="StaticBlock">
  <properties>
   <property name="rigid" type="bool" value="false"/>
  </properties>
  <animation>
   <frame tileid="4" duration="300"/>
   <frame tileid="5" duration="300"/>
   <frame tileid="6" duration="300"/>
   <frame tileid="7" duration="300"/>
  </animation>
 </tile>
 <tile id="8" type="StaticBlock">
  <properties>
   <property name="rigid" type="bool" value="false"/>
  </properties>
  <animation>
   <frame tileid="8" duration="1000"/>
   <frame tileid="9" duration="1000"/>
   <frame tileid="10" duration="1000"/>
   <frame tileid="11" duration="1000"/>
  </animation>
 </tile>
</tileset>
