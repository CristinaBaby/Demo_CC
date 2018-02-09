<GameProjectFile>
  <PropertyGroup Type="Scene" Name="pop_yesno" ID="24da4d73-5cfb-4c2f-9bcf-7b3e1585d587" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="55" Speed="1.0000" ActivedAnimationName="in">
        <Timeline ActionTag="-1904973358" Property="Position">
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
        <Timeline ActionTag="-1904973358" Property="Scale">
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
        <Timeline ActionTag="-1904973358" Property="RotationSkew">
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
          <RenderColor A="255" R="222" G="184" B="135" />
        </AnimationInfo>
        <AnimationInfo Name="out" StartIndex="30" EndIndex="55">
          <RenderColor A="255" R="255" G="182" B="193" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="74" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="Panel_3" ActionTag="1152666221" Tag="98" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" BackColorAlpha="152" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
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
          <AbstractNodeData Name="optionLayer" ActionTag="-523223496" Tag="75" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="popup_bg" ActionTag="-1904973358" Tag="78" IconVisible="False" LeftMargin="153.0000" RightMargin="153.0000" TopMargin="117.0000" BottomMargin="117.0000" ctype="SpriteObjectData">
                <Size X="654.0000" Y="406.0000" />
                <Children>
                  <AbstractNodeData Name="pop_Text" ActionTag="281694860" Tag="81" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="59.5000" RightMargin="59.5000" TopMargin="59.6516" BottomMargin="166.3484" IsCustomSize="True" FontSize="42" LabelText="Are you sure&#xA; you want reset ?" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="535.0000" Y="180.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="327.0000" Y="256.3484" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="188" G="7" B="209" />
                    <PrePosition X="0.5000" Y="0.6314" />
                    <PreSize X="0.8180" Y="0.4433" />
                    <FontResource Type="Normal" Path="ui/studio_fonts/LuckiestGuy.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_no" ActionTag="-2130085086" Tag="79" IconVisible="False" LeftMargin="370.8700" RightMargin="147.1300" TopMargin="259.8700" BottomMargin="62.1300" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="106" Scale9Height="62" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="136.0000" Y="84.0000" />
                    <Children>
                      <AbstractNodeData Name="Audio" ActionTag="-490584244" Tag="1057" IconVisible="True" RightMargin="960.0000" TopMargin="640.0000" Volume="0.5000" ctype="SimpleAudioObjectData">
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
                    <Position X="438.8700" Y="104.1300" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6711" Y="0.2565" />
                    <PreSize X="0.2080" Y="0.2069" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <PressedFileData Type="Normal" Path="ui/pubilc/no.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/pubilc/no.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_yes" ActionTag="-331683691" Tag="80" IconVisible="False" LeftMargin="147.3700" RightMargin="370.6300" TopMargin="259.8700" BottomMargin="62.1300" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="106" Scale9Height="62" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="136.0000" Y="84.0000" />
                    <Children>
                      <AbstractNodeData Name="Audio" ActionTag="-293062630" Tag="686" IconVisible="True" LeftMargin="0.0000" RightMargin="960.0000" TopMargin="640.0000" BottomMargin="0.0000" Volume="0.5000" ctype="SimpleAudioObjectData">
                        <Size X="0.0000" Y="0.0000" />
                        <AnchorPoint />
                        <Position X="0.0000" Y="0.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0000" Y="0.0000" />
                        <PreSize X="0.0000" Y="0.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="215.3700" Y="104.1300" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3293" Y="0.2565" />
                    <PreSize X="0.2080" Y="0.2069" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <PressedFileData Type="Normal" Path="ui/pubilc/yes.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/pubilc/yes.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6812" Y="0.6344" />
                <FileData Type="Normal" Path="ui/pubilc/tips.png" Plist="" />
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