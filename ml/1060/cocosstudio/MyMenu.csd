<GameProjectFile>
  <PropertyGroup Type="Scene" Name="MyMenu" ID="5485fc86-a577-4660-8c94-d981242b5a13" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="22" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="Panel_8" ActionTag="247975547" Tag="62" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" BackColorAlpha="136" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg" ActionTag="-1508400105" Tag="234" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="33.0000" RightMargin="33.0000" TopMargin="76.0000" BottomMargin="76.0000" TouchEnable="True" Scale9Width="894" Scale9Height="488" ctype="ImageViewObjectData">
            <Size X="894.0000" Y="488.0000" />
            <Children>
              <AbstractNodeData Name="Sprite_2" ActionTag="1033430466" Tag="97" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="345.0856" RightMargin="322.9144" TopMargin="10.1300" BottomMargin="427.8700" ctype="SpriteObjectData">
                <Size X="218.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="458.0856" Y="452.8700" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5124" Y="0.9280" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="ui/menu/menu-headline.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="optionLayer" ActionTag="866814119" Tag="30" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-33.0000" RightMargin="-33.0000" TopMargin="-76.0000" BottomMargin="-76.0000" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="960.0000" Y="640.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="447.0000" Y="244.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0738" Y="1.3115" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_close" ActionTag="1519355686" Tag="20" IconVisible="False" LeftMargin="790.5424" RightMargin="-38.5424" TopMargin="-29.7982" BottomMargin="375.7982" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="112" Scale9Height="120" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="142.0000" Y="142.0000" />
                <Children>
                  <AbstractNodeData Name="Audio" ActionTag="-379466193" Tag="1042" IconVisible="True" RightMargin="960.0000" TopMargin="640.0000" Volume="1.0000" ctype="SimpleAudioObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="sound/general/pop_up_close.mp3" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="861.5424" Y="446.7982" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9637" Y="0.9156" />
                <PreSize X="0.1588" Y="0.2910" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="ui/pubilc/close.png" Plist="" />
                <NormalFileData Type="Normal" Path="ui/pubilc/close.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.9312" Y="0.7625" />
            <FileData Type="Normal" Path="ui/menu/menu-bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>