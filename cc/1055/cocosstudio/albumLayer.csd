<GameFile>
  <PropertyGroup Name="albumLayer" Type="Scene" ID="7f326fb0-7b7d-4f7f-a2ce-8f3803ec1f2a" Version="2.3.3.0" />
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
          <AbstractNodeData Name="Panel_3" ActionTag="-64254065" Tag="261" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" BackColorAlpha="152" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="optionLayer" ActionTag="456253089" Tag="245" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="popup_bg" ActionTag="-1110485541" Tag="246" IconVisible="False" LeftMargin="167.0000" RightMargin="167.0000" TopMargin="102.0000" BottomMargin="102.0000" ctype="SpriteObjectData">
                <Size X="626.0000" Y="436.0000" />
                <Children>
                  <AbstractNodeData Name="contanier" ActionTag="713363756" Tag="253" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="45.0000" RightMargin="45.0000" TopMargin="69.3700" BottomMargin="42.6300" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="536.0000" Y="324.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="313.0000" Y="42.6300" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.0978" />
                    <PreSize X="0.8562" Y="0.7431" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="album_bg_4" ActionTag="133523159" Tag="254" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" ctype="SpriteObjectData">
                    <Size X="626.0000" Y="436.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="313.0000" Y="218.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="UI/album/photo.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_close" ActionTag="2122877254" Tag="250" IconVisible="False" LeftMargin="567.9611" RightMargin="18.0389" TopMargin="32.1085" BottomMargin="362.8915" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="11" BottomEage="11" Scale9OriginX="13" Scale9OriginY="11" Scale9Width="14" Scale9Height="19" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="40.0000" Y="41.0000" />
                    <Children>
                      <AbstractNodeData Name="Audio" ActionTag="1545772364" Tag="831" IconVisible="True" LeftMargin="89.0000" RightMargin="-49.0000" TopMargin="-35.9999" BottomMargin="76.9999" Volume="1.0000" ctype="SimpleAudioObjectData">
                        <Size X="0.0000" Y="0.0000" />
                        <AnchorPoint />
                        <Position X="89.0000" Y="76.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="2.2250" Y="1.8780" />
                        <PreSize X="0.0000" Y="0.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="587.9611" Y="383.3915" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9392" Y="0.8793" />
                    <PreSize X="0.0639" Y="0.0940" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="UI/album/close-btn.png" Plist="" />
                    <PressedFileData Type="Normal" Path="UI/album/close-btn.png" Plist="" />
                    <NormalFileData Type="Normal" Path="UI/album/close-btn.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_ablum" ActionTag="-2111465868" Tag="251" IconVisible="False" LeftMargin="62.0589" RightMargin="369.9411" TopMargin="379.4565" BottomMargin="-31.4565" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="164" Scale9Height="66" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="194.0000" Y="88.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="159.0589" Y="12.5435" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2541" Y="0.0288" />
                    <PreSize X="0.3099" Y="0.2018" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="UI/album/download-btn.png" Plist="" />
                    <PressedFileData Type="Normal" Path="UI/album/download-btn.png" Plist="" />
                    <NormalFileData Type="Normal" Path="UI/album/download-btn.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_share" ActionTag="932761938" Tag="252" IconVisible="False" LeftMargin="403.1530" RightMargin="28.8470" TopMargin="381.4565" BottomMargin="-33.4565" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="164" Scale9Height="66" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="194.0000" Y="88.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="500.1530" Y="10.5435" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7990" Y="0.0242" />
                    <PreSize X="0.3099" Y="0.2018" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="UI/album/mail-btn.png" Plist="" />
                    <PressedFileData Type="Normal" Path="UI/album/mail-btn.png" Plist="" />
                    <NormalFileData Type="Normal" Path="UI/album/mail-btn.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="320.0000" />
                <Scale />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6521" Y="0.6812" />
                <FileData Type="Normal" Path="UI/album/photo.png" Plist="" />
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
</GameFile>