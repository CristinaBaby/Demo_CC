<GameProjectFile>
  <PropertyGroup Type="Scene" Name="albumLayer" ID="7f326fb0-7b7d-4f7f-a2ce-8f3803ec1f2a" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="55" Speed="1.0000" ActivedAnimationName="in">
        <Timeline ActionTag="-1110485541" Property="Position">
          <PointFrame FrameIndex="0" X="480.0000" Y="320.0000">
            <EasingData Type="26" />
          </PointFrame>
          <PointFrame FrameIndex="25" X="480.0000" Y="320.0000">
            <EasingData Type="26" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="480.0000" Y="320.0000">
            <EasingData Type="25" />
          </PointFrame>
          <PointFrame FrameIndex="55" X="480.0000" Y="320.0000">
            <EasingData Type="25" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1110485541" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="25" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="55" X="0.0000" Y="0.0000">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1110485541" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="25" X="0.0000" Y="0.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="55" X="0.0000" Y="0.0000">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="in" StartIndex="0" EndIndex="25">
          <RenderColor A="255" R="70" G="130" B="180" />
        </AnimationInfo>
        <AnimationInfo Name="out" StartIndex="30" EndIndex="55">
          <RenderColor A="255" R="188" G="143" B="143" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="231" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="Panel_3" ActionTag="-64254065" Tag="261" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" BackColorAlpha="152" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="optionLayer" ActionTag="456253089" Tag="245" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="popup_bg" ActionTag="-1110485541" Tag="246" IconVisible="False" LeftMargin="71.5000" RightMargin="71.5000" TopMargin="23.0000" BottomMargin="23.0000" ctype="SpriteObjectData">
                <Size X="817.0000" Y="594.0000" />
                <Children>
                  <AbstractNodeData Name="contanier" ActionTag="713363756" Tag="253" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="41.7647" RightMargin="43.2353" TopMargin="65.3700" BottomMargin="45.6300" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                    <Size X="732.0000" Y="483.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="407.7647" Y="45.6300" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4991" Y="0.0768" />
                    <PreSize X="0.8960" Y="0.8131" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="album_bg_4" ActionTag="133523159" Tag="254" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" ctype="SpriteObjectData">
                    <Size X="817.0000" Y="594.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="408.5000" Y="297.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="ui/album/album_bg.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_close" ActionTag="2122877254" Tag="250" IconVisible="False" LeftMargin="705.9612" RightMargin="-30.9612" TopMargin="-10.3915" BottomMargin="462.3915" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="112" Scale9Height="120" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="142.0000" Y="142.0000" />
                    <Children>
                      <AbstractNodeData Name="Audio" ActionTag="1545772364" Tag="831" IconVisible="True" LeftMargin="0.0000" RightMargin="960.0000" TopMargin="640.0000" BottomMargin="0.0000" Volume="1.0000" ctype="SimpleAudioObjectData">
                        <Size X="0.0000" Y="0.0000" />
                        <AnchorPoint />
                        <Position X="0.0000" Y="0.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0000" Y="0.0000" />
                        <PreSize X="0.0000" Y="0.0000" />
                        <FileData Type="Normal" Path="sound/general/pop_up_close.mp3" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="776.9612" Y="533.3915" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9510" Y="0.8980" />
                    <PreSize X="0.1738" Y="0.2391" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="ui/pubilc/close.png" Plist="" />
                    <PressedFileData Type="Normal" Path="ui/pubilc/close.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/pubilc/close.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_ablum" ActionTag="-2111465868" Tag="251" IconVisible="False" LeftMargin="157.0589" RightMargin="533.9411" TopMargin="422.4565" BottomMargin="45.5435" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="96" Scale9Height="104" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="126.0000" Y="126.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.0589" Y="108.5435" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2693" Y="0.1827" />
                    <PreSize X="0.1425" Y="0.2258" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="ui/album/album_btn_album.png" Plist="" />
                    <PressedFileData Type="Normal" Path="ui/album/album_btn_album.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/album/album_btn_album.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_share" ActionTag="932761938" Tag="252" IconVisible="False" LeftMargin="539.1530" RightMargin="151.8470" TopMargin="422.4565" BottomMargin="45.5435" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="96" Scale9Height="104" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="126.0000" Y="126.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="602.1530" Y="108.5435" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7370" Y="0.1827" />
                    <PreSize X="0.1425" Y="0.2258" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="ui/album/album_btn_email.png" Plist="" />
                    <PressedFileData Type="Normal" Path="ui/album/album_btn_email.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/album/album_btn_email.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8510" Y="0.9281" />
                <FileData Type="Normal" Path="ui/album/album_bg.png" Plist="" />
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
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>