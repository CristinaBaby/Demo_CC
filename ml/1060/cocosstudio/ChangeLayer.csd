<GameProjectFile>
  <PropertyGroup Type="Scene" Name="ChangeLayer" ID="95d01707-69e3-4406-b994-532df169cd73" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="show">
        <Timeline ActionTag="-905155331" Property="Position">
          <PointFrame FrameIndex="0" X="-200.0000" Y="130.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="90" X="480.0000" Y="130.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-905155331" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-905155331" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-247803334" Property="Position">
          <PointFrame FrameIndex="0" X="-200.0000" Y="256.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="90" X="480.0000" Y="256.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-247803334" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-247803334" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="720.0000" Y="720.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="90">
          <RenderColor A="255" R="65" G="105" B="225" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="78" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="Image_1" ActionTag="-1478209452" Tag="677" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-88.0000" RightMargin="-88.0000" TopMargin="-64.0000" BottomMargin="-64.0000" TouchEnable="True" Scale9Width="1136" Scale9Height="768" ctype="ImageViewObjectData">
            <Size X="1136.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.1833" Y="1.2000" />
            <FileData Type="Normal" Path="png/loading/bg.jpg" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="optionLayer" ActionTag="1002102249" Tag="116" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="shade" ActionTag="-905155331" Tag="121" IconVisible="False" LeftMargin="-286.0000" RightMargin="1074.0000" TopMargin="490.5000" BottomMargin="110.5000" ctype="SpriteObjectData">
                <Size X="172.0000" Y="39.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="-200.0000" Y="130.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="-0.2083" Y="0.2031" />
                <PreSize X="0.2073" Y="0.0703" />
                <FileData Type="Normal" Path="png/loading/donuts11.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="donut" ActionTag="-247803334" Tag="122" IconVisible="False" LeftMargin="-286.0000" RightMargin="1074.0000" TopMargin="298.0000" BottomMargin="170.0000" ctype="SpriteObjectData">
                <Size X="172.0000" Y="172.0000" />
                <Children>
                  <AbstractNodeData Name="icing" ActionTag="-383046986" Alpha="0" Tag="46" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="-160.0000" RightMargin="-160.0000" TopMargin="-157.0000" BottomMargin="-157.0000" ctype="SpriteObjectData">
                    <Size X="176.0000" Y="183.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="86.0000" Y="86.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="png/1/donuts.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="donuts1" ActionTag="963638997" Tag="126" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" ctype="SpriteObjectData">
                    <Size X="172.0000" Y="172.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="86.0000" Y="86.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="png/loading/donuts1.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="donuts0" ActionTag="-1939618474" VisibleForFrame="False" Tag="127" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" ctype="SpriteObjectData">
                    <Size X="172.0000" Y="172.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="86.0000" Y="86.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="png/loading/donuts0.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="-200.0000" Y="256.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="-0.2083" Y="0.4000" />
                <PreSize X="0.1792" Y="0.2688" />
                <FileData Type="Normal" Path="png/loading/donuts2.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="limit" ActionTag="226849260" Tag="123" IconVisible="False" LeftMargin="380.0000" RightMargin="380.0000" TopMargin="450.6815" BottomMargin="-10.6815" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="200.0000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="480.0000" Y="-10.6815" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="-0.0167" />
                <PreSize X="0.0000" Y="0.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
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