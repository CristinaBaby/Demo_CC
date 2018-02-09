<GameProjectFile>
  <PropertyGroup Type="Scene" Name="writeLayer" ID="64b371d2-8b4a-44f0-8d9a-285578a90b0a" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="614001206" Property="Position">
          <PointFrame FrameIndex="30" X="402.0000" Y="-36.6800">
            <EasingData Type="29" />
          </PointFrame>
          <PointFrame FrameIndex="45" X="402.0000" Y="-142.5400">
            <EasingData Type="16" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="402.0000" Y="-36.6800">
            <EasingData Type="29" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="614001206" Property="Scale">
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="29" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="45" X="1.0000" Y="1.0000">
            <EasingData Type="16" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="29" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="614001206" Property="RotationSkew">
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="29" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="45" X="0.0000" Y="0.0000">
            <EasingData Type="16" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.0000" Y="0.0000">
            <EasingData Type="29" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="showIcon" StartIndex="30" EndIndex="45">
          <RenderColor A="255" R="189" G="183" B="107" />
        </AnimationInfo>
        <AnimationInfo Name="hideIcon" StartIndex="45" EndIndex="60">
          <RenderColor A="255" R="255" G="20" B="147" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="46" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="writeLayer" CanEdit="False" ActionTag="623787429" Tag="47" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="layer" CanEdit="False" ActionTag="-60543890" Tag="48" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-88.0000" RightMargin="-88.0000" TopMargin="-64.0000" BottomMargin="-64.0000" TouchEnable="True" BackColorAlpha="150" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="1136.0000" Y="768.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.1833" Y="1.2000" />
                <SingleColor A="255" R="0" G="0" B="0" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="container" CanEdit="False" ActionTag="2040302373" Tag="49" IconVisible="False" LeftMargin="85.0000" RightMargin="85.0000" TopMargin="103.0000" BottomMargin="297.0000" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="790.0000" Y="240.0000" />
                <Children>
                  <AbstractNodeData Name="iconBg" ActionTag="614001206" Tag="55" IconVisible="False" LeftMargin="-17.0000" RightMargin="-31.0000" TopMargin="224.6800" BottomMargin="-88.6800" TouchEnable="True" Scale9Width="838" Scale9Height="104" ctype="ImageViewObjectData">
                    <Size X="838.0000" Y="104.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="402.0000" Y="-36.6800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5089" Y="-0.1528" />
                    <PreSize X="1.0608" Y="0.4333" />
                    <FileData Type="Normal" Path="ui/draw/hurdle.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="signature1" ActionTag="-1323505229" Tag="50" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-41.5430" RightMargin="-59.4570" TopMargin="22.8000" BottomMargin="17.2000" ctype="SpriteObjectData">
                    <Size X="790.0000" Y="240.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="405.2700" Y="117.6000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5130" Y="0.4900" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="ui/draw/bg-draw2.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_1" ActionTag="1382327944" Tag="114" IconVisible="False" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                    <Size X="790.0000" Y="240.0000" />
                    <Children>
                      <AbstractNodeData Name="renderLayer" ActionTag="513725110" Tag="51" IconVisible="False" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                        <Size X="790.0000" Y="240.0000" />
                        <Children>
                          <AbstractNodeData Name="bgSprite" ActionTag="295524227" Tag="321" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-173.0000" RightMargin="-173.0000" TopMargin="5.0000" BottomMargin="5.0000" ctype="SpriteObjectData">
                            <Size X="1136.0000" Y="230.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="395.0000" Y="120.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.0000" Y="0.0000" />
                            <FileData Type="Normal" Path="png/pattern.png" Plist="" />
                            <BlendFunc Src="1" Dst="771" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition />
                        <PreSize X="1.0000" Y="1.0000" />
                        <SingleColor A="255" R="150" G="200" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="-614588340" Tag="117" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-55.0000" RightMargin="-55.0000" TopMargin="-102.0000" BottomMargin="-102.0000" Scale9Width="900" Scale9Height="444" ctype="ImageViewObjectData">
                    <Size X="900.0000" Y="444.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="395.0000" Y="120.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7923" Y="0.5781" />
                    <FileData Type="Normal" Path="ui/draw/bg-draw.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="wirteUI" ActionTag="-445452890" VisibleForFrame="False" Tag="52" IconVisible="False" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                    <Size X="790.0000" Y="240.0000" />
                    <Children>
                      <AbstractNodeData Name="btn_sure" ActionTag="-1837502112" Tag="53" IconVisible="False" LeftMargin="717.8838" RightMargin="-53.8838" TopMargin="-103.7822" BottomMargin="217.7822" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="96" Scale9Height="104" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="126.0000" Y="126.0000" />
                        <Children>
                          <AbstractNodeData Name="Audio" ActionTag="322810163" Tag="1174" IconVisible="True" RightMargin="960.0000" TopMargin="640.0000" Volume="1.0000" ctype="SimpleAudioObjectData">
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
                        <Position X="780.8838" Y="280.7822" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.9885" Y="1.1699" />
                        <PreSize X="0.1595" Y="0.5250" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Normal" Path="ui/draw/confirm.png" Plist="" />
                        <PressedFileData Type="Normal" Path="ui/draw/confirm.png" Plist="" />
                        <NormalFileData Type="Normal" Path="ui/draw/confirm.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btn_delete" ActionTag="638021724" Tag="54" IconVisible="False" LeftMargin="-41.1710" RightMargin="705.1710" TopMargin="-103.7822" BottomMargin="217.7822" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="92" Scale9Height="98" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="126.0000" Y="126.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="21.8290" Y="280.7822" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0276" Y="1.1699" />
                        <PreSize X="0.1595" Y="0.5250" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Normal" Path="ui/draw/delete.png" Plist="" />
                        <PressedFileData Type="Normal" Path="ui/draw/delete.png" Plist="" />
                        <NormalFileData Type="Normal" Path="ui/draw/delete.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="chooseBg" ActionTag="-329017142" Tag="116" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="15.0000" RightMargin="15.0000" TopMargin="225.8800" BottomMargin="-84.8800" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                        <Size X="760.0000" Y="99.0000" />
                        <Children>
                          <AbstractNodeData Name="color" ActionTag="-1740049314" Tag="58" IconVisible="False" LeftMargin="66.4688" RightMargin="603.5313" TopMargin="-7.2213" BottomMargin="4.2213" TouchEnable="True" ctype="CheckBoxObjectData">
                            <Size X="90.0000" Y="102.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="111.4688" Y="55.2213" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.1467" Y="0.5578" />
                            <PreSize X="0.1184" Y="1.0303" />
                            <NormalBackFileData Type="Normal" Path="ui/draw/brush.png" Plist="" />
                            <PressedBackFileData Type="Normal" Path="ui/draw/brush.png" Plist="" />
                            <DisableBackFileData Type="Default" Path="Default/CheckBox_Disable.png" Plist="" />
                            <NodeNormalFileData Type="Normal" Path="ui/draw/brush.png" Plist="" />
                            <NodeDisableFileData Type="Normal" Path="ui/draw/brush.png" Plist="" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="pattern" ActionTag="-457148518" Tag="59" IconVisible="False" LeftMargin="258.5860" RightMargin="427.4140" TopMargin="0.7787" BottomMargin="12.2213" TouchEnable="True" ctype="CheckBoxObjectData">
                            <Size X="74.0000" Y="86.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="295.5860" Y="55.2213" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.3889" Y="0.5578" />
                            <PreSize X="0.0974" Y="0.8687" />
                            <NormalBackFileData Type="Normal" Path="ui/draw/pattern.png" Plist="" />
                            <PressedBackFileData Type="Normal" Path="ui/draw/pattern.png" Plist="" />
                            <DisableBackFileData Type="Default" Path="Default/CheckBox_Disable.png" Plist="" />
                            <NodeNormalFileData Type="Normal" Path="ui/draw/pattern.png" Plist="" />
                            <NodeDisableFileData Type="Normal" Path="ui/draw/pattern.png" Plist="" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="pencil" ActionTag="292987373" Tag="61" IconVisible="False" LeftMargin="449.7000" RightMargin="244.3000" TopMargin="1.7785" BottomMargin="13.2215" TouchEnable="True" ctype="CheckBoxObjectData">
                            <Size X="66.0000" Y="84.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="482.7000" Y="55.2215" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.6351" Y="0.5578" />
                            <PreSize X="0.0868" Y="0.8485" />
                            <NormalBackFileData Type="Normal" Path="ui/draw/pencil.png" Plist="" />
                            <PressedBackFileData Type="Normal" Path="ui/draw/pencil.png" Plist="" />
                            <DisableBackFileData Type="Default" Path="Default/CheckBox_Disable.png" Plist="" />
                            <NodeNormalFileData Type="Normal" Path="ui/draw/pencil.png" Plist="" />
                            <NodeDisableFileData Type="Normal" Path="ui/draw/pencil.png" Plist="" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="stickers" ActionTag="-603160853" Tag="60" IconVisible="False" LeftMargin="617.8200" RightMargin="46.1800" TopMargin="2.7787" BottomMargin="14.2213" TouchEnable="True" ctype="CheckBoxObjectData">
                            <Size X="96.0000" Y="82.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="665.8200" Y="55.2213" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.8761" Y="0.5578" />
                            <PreSize X="0.1263" Y="0.8283" />
                            <NormalBackFileData Type="Normal" Path="ui/draw/tags.png" Plist="" />
                            <PressedBackFileData Type="Normal" Path="ui/draw/tags.png" Plist="" />
                            <DisableBackFileData Type="Default" Path="Default/CheckBox_Disable.png" Plist="" />
                            <NodeNormalFileData Type="Normal" Path="ui/draw/tags.png" Plist="" />
                            <NodeDisableFileData Type="Normal" Path="ui/draw/tags.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" />
                        <Position X="395.0000" Y="-84.8800" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="-0.3537" />
                        <PreSize X="0.9620" Y="0.4125" />
                        <SingleColor A="255" R="150" G="200" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="417.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6516" />
                <PreSize X="0.8229" Y="0.3750" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="circle" ActionTag="-1029179073" VisibleForFrame="False" Tag="122" IconVisible="False" LeftMargin="413.0724" RightMargin="488.9276" TopMargin="178.5379" BottomMargin="403.4621" ctype="SpriteObjectData">
                <Size X="58.0000" Y="58.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="442.0724" Y="432.4621" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4605" Y="0.6757" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="ui/pubilc/finger2-bg.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="finger" ActionTag="-680216596" VisibleForFrame="False" Tag="123" RotationSkewY="0.0017" IconVisible="False" LeftMargin="450.9377" RightMargin="405.0623" TopMargin="176.3689" BottomMargin="343.6311" ctype="SpriteObjectData">
                <Size X="104.0000" Y="120.0000" />
                <AnchorPoint ScaleX="0.7464" ScaleY="0.2864" />
                <Position X="528.5644" Y="378.0037" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5506" Y="0.5906" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="ui/pubilc/finger2.png" Plist="" />
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