<GameProjectFile>
  <PropertyGroup Type="Scene" Name="rollStep" ID="43d5e659-fc42-4184-bba2-6f148b1afbc9" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="54" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="2111253931" Tag="56" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-88.0000" RightMargin="-88.0000" TopMargin="-64.0000" BottomMargin="-64.0000" ctype="SpriteObjectData">
            <Size X="1136.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="png/3/bg.jpg" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="optionLayer" ActionTag="374179127" Tag="57" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="flour_11" ActionTag="47533233" Tag="99" IconVisible="False" LeftMargin="86.3549" RightMargin="92.6451" TopMargin="43.4694" BottomMargin="-3.4694" ctype="SpriteObjectData">
                <Size X="781.0000" Y="600.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="476.8549" Y="296.5306" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4967" Y="0.4633" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="png/3/flour.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="dough" ActionTag="722362085" Tag="95" IconVisible="False" LeftMargin="237.3987" RightMargin="252.6013" TopMargin="41.6945" BottomMargin="93.3055" ctype="SpriteObjectData">
                <Size X="470.0000" Y="505.0000" />
                <Children>
                  <AbstractNodeData Name="Audio" ActionTag="920263438" Tag="597" IconVisible="True" RightMargin="960.0000" TopMargin="640.0000" Volume="1.0000" Loop="True" ctype="SimpleAudioObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="sound/general/roll the dough.mp3" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="472.3987" Y="345.8055" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4921" Y="0.5403" />
                <PreSize X="0.4896" Y="0.7891" />
                <FileData Type="Normal" Path="png/3/dough-rolling_0.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="cutter" ActionTag="-325825247" VisibleForFrame="False" Tag="97" IconVisible="False" LeftMargin="56.0000" RightMargin="788.0000" TopMargin="34.7329" BottomMargin="491.2671" ctype="SpriteObjectData">
                <Size X="116.0000" Y="114.0000" />
                <Children>
                  <AbstractNodeData Name="Audio" ActionTag="-481473047" Tag="598" IconVisible="True" LeftMargin="10.0001" RightMargin="459.9999" TopMargin="515.0000" BottomMargin="-10.0000" Volume="1.0000" ctype="SimpleAudioObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint />
                    <Position X="10.0001" Y="-10.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0213" Y="-0.0198" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="sound/general/cut.mp3" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="114.0000" Y="548.2671" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1187" Y="0.8567" />
                <PreSize X="0.1208" Y="0.1781" />
                <FileData Type="Normal" Path="png/3/dough_tool.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="tool" ActionTag="-1513099569" VisibleForFrame="False" Tag="96" IconVisible="False" LeftMargin="676.7000" RightMargin="175.3000" TopMargin="19.8002" BottomMargin="136.1998" ctype="SpriteObjectData">
                <Size X="108.0000" Y="484.0000" />
                <Children>
                  <AbstractNodeData Name="noshade" ActionTag="872947939" Tag="89" RotationSkewY="-0.0017" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="0.0022" RightMargin="-0.0022" TopMargin="-0.0009" BottomMargin="0.0009" ctype="SpriteObjectData">
                    <Size X="108.0000" Y="484.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="54.0000" Y="242.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="png/3/rolling-pin.png" Plist="" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="730.7000" Y="378.1998" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7611" Y="0.5909" />
                <PreSize X="0.1125" Y="0.7563" />
                <FileData Type="Normal" Path="png/3/rolling-pin_shadow.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="2062978316" VisibleForFrame="False" Tag="167" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BottomEdge" LeftMargin="187.0000" RightMargin="187.0000" TopMargin="490.0000" BottomMargin="100.0000" ctype="SpriteObjectData">
            <Size X="586.0000" Y="50.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="125.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.1953" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/cut/cut.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="btn_next" ActionTag="1247088899" VisibleForFrame="False" Tag="170" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="819.5740" RightMargin="14.4260" TopMargin="9.9892" BottomMargin="504.0108" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="96" Scale9Height="104" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="126.0000" Y="126.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="882.5740" Y="567.0108" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9193" Y="0.8860" />
            <PreSize X="0.1312" Y="0.1969" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Normal" Path="ui/pubilc/next.png" Plist="" />
            <PressedFileData Type="Normal" Path="ui/pubilc/next.png" Plist="" />
            <NormalFileData Type="Normal" Path="ui/pubilc/next.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="btn_reset" ActionTag="1271173388" VisibleForFrame="False" Tag="171" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="819.5740" RightMargin="14.4260" TopMargin="124.1735" BottomMargin="389.8265" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="96" Scale9Height="104" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="126.0000" Y="126.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="882.5740" Y="452.8265" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9193" Y="0.7075" />
            <PreSize X="0.1312" Y="0.1969" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Normal" Path="ui/pubilc/reset.png" Plist="" />
            <PressedFileData Type="Normal" Path="ui/pubilc/reset.png" Plist="" />
            <NormalFileData Type="Normal" Path="ui/pubilc/reset.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="title_v" ActionTag="-1837740924" VisibleForFrame="False" Tag="172" IconVisible="False" LeftMargin="261.5172" RightMargin="640.4828" TopMargin="142.3533" BottomMargin="223.6467" ctype="SpriteObjectData">
            <Size X="58.0000" Y="274.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="290.5172" Y="360.6467" />
            <Scale ScaleX="1.0000" ScaleY="0.4000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.3026" Y="0.5635" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/rolldough/arrows-1.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="title_h" ActionTag="-1162748150" VisibleForFrame="False" Tag="173" IconVisible="False" LeftMargin="305.7923" RightMargin="380.2077" TopMargin="378.5849" BottomMargin="203.4151" ctype="SpriteObjectData">
            <Size X="274.0000" Y="58.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="442.7923" Y="232.4151" />
            <Scale ScaleX="0.4000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4612" Y="0.3631" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/rolldough/arrows-2.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>