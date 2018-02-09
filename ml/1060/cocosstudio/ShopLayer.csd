<GameProjectFile>
  <PropertyGroup Type="Scene" Name="ShopLayer" ID="21a36bdf-5b5a-4a19-baee-ce9fb399e44e" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="491" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="lockLayer" ActionTag="597049617" Tag="492" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" BackColorAlpha="170" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
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
          <AbstractNodeData Name="bg" ActionTag="-502556586" Tag="509" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" ctype="SpriteObjectData">
            <Size X="1136.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/shop/shop_bg.jpg" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="btn_restore" ActionTag="1462877342" Tag="511" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="-56.0000" RightMargin="636.0000" TopMargin="398.0000" BottomMargin="-56.0000" TouchEnable="True" Scale9Width="380" Scale9Height="298" ctype="ImageViewObjectData">
            <Size X="380.0000" Y="298.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="134.0000" Y="93.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1396" Y="0.1453" />
            <PreSize X="0.3958" Y="0.4656" />
            <FileData Type="Normal" Path="ui/shop/restore.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="optionLayer" ActionTag="-404430471" Tag="502" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="light" ActionTag="-2083440382" Tag="514" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="219.0000" RightMargin="219.0000" TopMargin="-96.8000" BottomMargin="32.8000" ctype="SpriteObjectData">
                <Size X="522.0000" Y="704.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="384.8000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6012" />
                <PreSize X="0.5437" Y="1.1000" />
                <FileData Type="Normal" Path="ui/shop/shop-light.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="decorations" ActionTag="-736566154" VisibleForFrame="False" Tag="582" IconVisible="False" LeftMargin="-146.3513" RightMargin="678.3513" TopMargin="243.3879" BottomMargin="86.6121" ctype="SpriteObjectData">
                <Size X="428.0000" Y="310.0000" />
                <Children>
                  <AbstractNodeData Name="1" ActionTag="-297256155" Tag="584" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="135.1918" RightMargin="68.8082" TopMargin="211.0813" BottomMargin="4.9187" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="194" Scale9Height="72" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="224.0000" Y="94.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="247.2128" Y="51.9187" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5776" Y="0.1675" />
                    <PreSize X="0.2333" Y="0.1469" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <PressedFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="alldecorations_50" ActionTag="-1060231122" Tag="583" IconVisible="False" LeftMargin="156.0690" RightMargin="23.9310" TopMargin="-12.3727" BottomMargin="278.3727" ctype="SpriteObjectData">
                    <Size X="248.0000" Y="44.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="280.0690" Y="300.3727" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6544" Y="0.9689" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="ui/shop/alldecorations.png" Plist="" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="67.6487" Y="241.6121" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0705" Y="0.3775" />
                <PreSize X="0.4458" Y="0.4844" />
                <FileData Type="Normal" Path="ui/shop/decorations.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="buyAll" ActionTag="1280485416" VisibleForFrame="False" Tag="519" IconVisible="False" LeftMargin="242.0000" RightMargin="242.0000" TopMargin="101.3896" BottomMargin="20.6104" ctype="SpriteObjectData">
                <Size X="476.0000" Y="518.0000" />
                <Children>
                  <AbstractNodeData Name="Particle_1" ActionTag="-454237229" Tag="515" IconVisible="True" LeftMargin="230.5942" RightMargin="245.4058" TopMargin="248.2686" BottomMargin="269.7314" ctype="ParticleObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint />
                    <Position X="230.5942" Y="269.7314" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4844" Y="0.5207" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="particle/starParticle.plist" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="fullversion_48" ActionTag="-925384231" Tag="581" IconVisible="False" LeftMargin="161.5168" RightMargin="116.4832" TopMargin="-17.2600" BottomMargin="493.2600" ctype="SpriteObjectData">
                    <Size X="198.0000" Y="42.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="260.5168" Y="514.2600" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5473" Y="0.9928" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="ui/shop/fullversion.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="0" ActionTag="1057542818" Tag="513" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="122.7966" RightMargin="129.2034" TopMargin="416.2376" BottomMargin="7.7624" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="194" Scale9Height="72" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="224.0000" Y="94.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="234.8108" Y="54.7624" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4933" Y="0.1057" />
                    <PreSize X="0.2333" Y="0.1469" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <PressedFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="279.6104" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4369" />
                <PreSize X="0.4958" Y="0.8094" />
                <FileData Type="Normal" Path="ui/shop/all.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="noAds" ActionTag="188598555" VisibleForFrame="False" Tag="585" IconVisible="False" LeftMargin="701.0768" RightMargin="-55.0768" TopMargin="295.2893" BottomMargin="120.7107" ctype="SpriteObjectData">
                <Size X="314.0000" Y="224.0000" />
                <Children>
                  <AbstractNodeData Name="removeads_52" ActionTag="589361311" Tag="586" IconVisible="False" LeftMargin="79.9878" RightMargin="52.0122" TopMargin="-64.2737" BottomMargin="248.2737" ctype="SpriteObjectData">
                    <Size X="182.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="170.9878" Y="268.2737" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5445" Y="1.1977" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="ui/shop/removeads.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="2" ActionTag="-1601981040" Tag="587" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="41.0285" RightMargin="48.9715" TopMargin="159.1802" BottomMargin="-29.1802" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="194" Scale9Height="72" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="224.0000" Y="94.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="153.0436" Y="17.8198" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4874" Y="0.0796" />
                    <PreSize X="0.2333" Y="0.1469" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <PressedFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <NormalFileData Type="Normal" Path="ui/shop/btn-buy.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="858.0768" Y="232.7107" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8938" Y="0.3636" />
                <PreSize X="0.3271" Y="0.3500" />
                <FileData Type="Normal" Path="ui/shop/no-ads.png" Plist="" />
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
          <AbstractNodeData Name="logo" ActionTag="-1138691864" Tag="510" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="TopEdge" LeftMargin="217.0000" RightMargin="217.0000" TopMargin="31.9500" BottomMargin="554.0500" ctype="SpriteObjectData">
            <Size X="526.0000" Y="54.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="581.0500" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.9079" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/shop/shop-headline.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="btn_close" ActionTag="-859283661" Tag="516" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="2.2448" RightMargin="831.7552" TopMargin="0.2068" BottomMargin="513.7932" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="96" Scale9Height="104" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="126.0000" Y="126.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="65.2448" Y="576.7932" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0680" Y="0.9012" />
            <PreSize X="0.1312" Y="0.1969" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Normal" Path="ui/pubilc/btn-back.png" Plist="" />
            <PressedFileData Type="Normal" Path="ui/pubilc/btn-back.png" Plist="" />
            <NormalFileData Type="Normal" Path="ui/pubilc/btn-back.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>