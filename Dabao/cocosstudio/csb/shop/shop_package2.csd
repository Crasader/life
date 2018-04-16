<GameFile>
  <PropertyGroup Name="shop_package2" Type="Node" ID="fe9aa9a8-36e3-4300-9ebf-6642a5e74624" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="40" Speed="0.3333">
        <Timeline ActionTag="-1710082438" Property="Position">
          <PointFrame FrameIndex="0" X="68.1201" Y="163.5778">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="68.1201" Y="163.5778">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="40" X="68.1201" Y="163.5778">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1710082438" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.9000" Y="0.9000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.2000" Y="1.2000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1710082438" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="4.0197" Y="4.0197">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="16.0200" Y="16.0200">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="4.0197" Y="4.0197">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1710082438" Property="Alpha">
          <IntFrame FrameIndex="0" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-374938599" Property="Position">
          <PointFrame FrameIndex="15" X="68.1201" Y="163.5778">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="40" X="68.1201" Y="163.5778">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-374938599" Property="Scale">
          <ScaleFrame FrameIndex="15" X="0.6000" Y="0.6000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="0.9000" Y="0.9000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-374938599" Property="RotationSkew">
          <ScaleFrame FrameIndex="15" X="-5.9800" Y="-5.9800">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="4.0197" Y="4.0197">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-374938599" Property="Alpha">
          <IntFrame FrameIndex="15" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="40" Value="255">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="play" StartIndex="0" EndIndex="40">
          <RenderColor A="150" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Node" Tag="135" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="bg_img" ActionTag="-541796071" Tag="136" IconVisible="False" LeftMargin="-90.0000" RightMargin="-90.0000" TopMargin="-150.0000" BottomMargin="-150.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="144" Scale9Height="294" ctype="ImageViewObjectData">
            <Size X="180.0000" Y="300.0000" />
            <Children>
              <AbstractNodeData Name="ok_button" ActionTag="1813462806" Tag="164" IconVisible="False" LeftMargin="-17.5000" RightMargin="-17.5000" TopMargin="229.0001" BottomMargin="-7.0001" TouchEnable="True" FontSize="36" ButtonText="购买" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="185" Scale9Height="56" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="215.0000" Y="78.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.0000" Y="31.9999" />
                <Scale ScaleX="0.4500" ScaleY="0.4500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1067" />
                <PreSize X="1.1944" Y="0.2600" />
                <FontResource Type="Normal" Path="ui/fonts/fonts2.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <PressedFileData Type="Normal" Path="ui/common/changanniu1hui.png" Plist="" />
                <NormalFileData Type="Normal" Path="ui/common/changanniu1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="name_text" ActionTag="-1321664977" Tag="137" IconVisible="False" LeftMargin="40.5000" RightMargin="40.5000" TopMargin="211.5000" BottomMargin="57.5000" FontSize="24" LabelText="宝石礼包" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="99.0000" Y="31.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.0000" Y="73.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2433" />
                <PreSize X="0.5500" Y="0.1033" />
                <FontResource Type="Normal" Path="ui/fonts/fonts2.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="icon" ActionTag="-262136207" Tag="139" IconVisible="False" LeftMargin="29.9001" RightMargin="35.0999" TopMargin="94.2721" BottomMargin="104.7279" LeftEage="23" RightEage="23" TopEage="18" BottomEage="18" Scale9OriginX="23" Scale9OriginY="18" Scale9Width="69" Scale9Height="65" ctype="ImageViewObjectData">
                <Size X="115.0000" Y="101.0000" />
                <AnchorPoint ScaleX="0.5252" ScaleY="0.5481" />
                <Position X="90.2981" Y="160.0860" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5017" Y="0.5336" />
                <PreSize X="0.6389" Y="0.3367" />
                <FileData Type="Normal" Path="ui/common/libaotu.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_12" ActionTag="1181711196" Tag="141" IconVisible="False" LeftMargin="6.4998" RightMargin="6.5002" TopMargin="244.0000" BottomMargin="54.0000" LeftEage="49" RightEage="49" Scale9OriginX="49" Scale9Width="52" Scale9Height="2" ctype="ImageViewObjectData">
                <Size X="167.0000" Y="2.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="89.9998" Y="55.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1833" />
                <PreSize X="0.9278" Y="0.0067" />
                <FileData Type="Normal" Path="ui/common/fengexian.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="detail_button" Visible="False" ActionTag="-1459179893" Tag="143" IconVisible="False" LeftMargin="25.6989" RightMargin="24.3011" TopMargin="35.6240" BottomMargin="123.3760" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="4" BottomEage="4" Scale9OriginX="-15" Scale9OriginY="-4" Scale9Width="30" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="130.0000" Y="141.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.6989" Y="193.8760" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5039" Y="0.6463" />
                <PreSize X="0.7222" Y="0.4700" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="shopshanguang_2" ActionTag="-1710082438" Tag="390" RotationSkewX="4.0197" RotationSkewY="4.0197" IconVisible="False" LeftMargin="-31.8799" RightMargin="11.8799" TopMargin="36.4222" BottomMargin="63.5778" ctype="SpriteObjectData">
                <Size X="200.0000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="68.1201" Y="163.5778" />
                <Scale ScaleX="0.9000" ScaleY="0.9000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3784" Y="0.5453" />
                <PreSize X="1.1111" Y="0.6667" />
                <FileData Type="Normal" Path="ui/shop/shopshanguang.png" Plist="" />
                <BlendFunc Src="770" Dst="1" />
              </AbstractNodeData>
              <AbstractNodeData Name="shopshanguang_2_0" ActionTag="-374938599" Alpha="0" Tag="30" RotationSkewX="-5.9800" RotationSkewY="-5.9800" IconVisible="False" LeftMargin="-31.8799" RightMargin="11.8799" TopMargin="36.4222" BottomMargin="63.5778" ctype="SpriteObjectData">
                <Size X="200.0000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="68.1201" Y="163.5778" />
                <Scale ScaleX="0.6000" ScaleY="0.6000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3784" Y="0.5453" />
                <PreSize X="1.1111" Y="0.6667" />
                <FileData Type="Normal" Path="ui/shop/shopshanguang.png" Plist="" />
                <BlendFunc Src="770" Dst="1" />
              </AbstractNodeData>
              <AbstractNodeData Name="price_bg" ActionTag="1267185307" Tag="145" IconVisible="False" LeftMargin="-25.0000" RightMargin="105.0000" TopMargin="-8.0000" BottomMargin="208.0000" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="33" BottomEage="33" Scale9OriginX="16" Scale9OriginY="33" Scale9Width="68" Scale9Height="34" ctype="ImageViewObjectData">
                <Size X="100.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_2" ActionTag="-1928504169" Tag="146" IconVisible="False" LeftMargin="29.3552" RightMargin="50.6448" TopMargin="54.5867" BottomMargin="23.4133" FontSize="16" LabelText="元" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="20.0000" Y="22.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="39.3552" Y="34.4133" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3936" Y="0.3441" />
                    <PreSize X="0.2000" Y="0.2200" />
                    <FontResource Type="Normal" Path="ui/fonts/fonts2.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="price_text" ActionTag="-1014624887" Tag="147" IconVisible="False" LeftMargin="29.3552" RightMargin="31.6448" TopMargin="37.6502" BottomMargin="40.3498" FontSize="16" LabelText="648" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="39.0000" Y="22.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="29.3552" Y="51.3498" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2936" Y="0.5135" />
                    <PreSize X="0.3900" Y="0.2200" />
                    <FontResource Type="Normal" Path="ui/fonts/fonts2.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1_2_0_0" ActionTag="-987500067" Tag="148" IconVisible="False" LeftMargin="30.0000" RightMargin="30.0000" TopMargin="17.0000" BottomMargin="59.0000" FontSize="18" LabelText="价值" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="40.0000" Y="24.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="71.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.7100" />
                    <PreSize X="0.4000" Y="0.2400" />
                    <FontResource Type="Normal" Path="ui/fonts/fonts2.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="25.0000" Y="258.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1389" Y="0.8600" />
                <PreSize X="0.5556" Y="0.3333" />
                <FileData Type="Normal" Path="ui/common/touming.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/shop/shopdi.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>