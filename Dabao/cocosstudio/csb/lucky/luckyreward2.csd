<GameFile>
  <PropertyGroup Name="luckyreward2" Type="Node" ID="6a003377-6656-4ff6-8832-814539478e22" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="16" Speed="0.5000" ActivedAnimationName="out">
        <Timeline ActionTag="200964836" Property="Position">
          <PointFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="7" X="-0.7266" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="10" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="11" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="16" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="200964836" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="7" X="1.0500" Y="1.0500">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="10" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="11" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="16" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="200964836" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="7" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="10" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="11" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="16" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="200964836" Property="Alpha">
          <IntFrame FrameIndex="11" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="16" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="in" StartIndex="0" EndIndex="10">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="out" StartIndex="11" EndIndex="16">
          <RenderColor A="255" R="169" G="169" B="169" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Node" Tag="65" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="bg_img" ActionTag="200964836" Tag="66" IconVisible="False" LeftMargin="-200.0000" RightMargin="-200.0000" TopMargin="-125.0000" BottomMargin="-125.0000" Scale9Enable="True" LeftEage="65" RightEage="65" TopEage="64" BottomEage="64" Scale9OriginX="65" Scale9OriginY="64" Scale9Width="670" Scale9Height="352" ctype="ImageViewObjectData">
            <Size X="400.0000" Y="250.0000" />
            <Children>
              <AbstractNodeData Name="close_button" ActionTag="-1681447228" Tag="69" IconVisible="False" LeftMargin="365.1473" RightMargin="-20.1473" TopMargin="-23.1096" BottomMargin="218.1096" TouchEnable="True" FontSize="20" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="25" Scale9Height="33" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="55.0000" Y="55.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="392.6473" Y="245.6096" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9816" Y="0.9824" />
                <PreSize X="0.1375" Y="0.2200" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="ui/common/fanhui.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_3" ActionTag="94632195" Tag="11197" IconVisible="False" LeftMargin="158.5920" RightMargin="158.4080" TopMargin="33.9011" BottomMargin="190.0989" LeftEage="14" RightEage="14" TopEage="8" BottomEage="8" Scale9OriginX="14" Scale9OriginY="8" Scale9Width="55" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="83.0000" Y="26.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="200.0920" Y="203.0989" />
                <Scale ScaleX="1.2000" ScaleY="1.2000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5002" Y="0.8124" />
                <PreSize X="0.2075" Y="0.1040" />
                <FileData Type="Normal" Path="ui/common/otherzi-29.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_10_0" ActionTag="346661773" Tag="529" IconVisible="False" LeftMargin="60.0296" RightMargin="276.9704" TopMargin="83.2100" BottomMargin="103.7900" LeftEage="29" RightEage="29" TopEage="29" BottomEage="29" Scale9OriginX="29" Scale9OriginY="29" Scale9Width="5" Scale9Height="5" ctype="ImageViewObjectData">
                <Size X="63.0000" Y="63.0000" />
                <Children>
                  <AbstractNodeData Name="Image_14" ActionTag="1510478966" Tag="55" IconVisible="False" LeftMargin="-0.2215" RightMargin="0.2215" TopMargin="-0.1905" BottomMargin="0.1905" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="37" Scale9Height="37" ctype="ImageViewObjectData">
                    <Size X="63.0000" Y="63.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="31.2785" Y="31.6905" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4965" Y="0.5030" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="ui/common/shimo2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1_0" ActionTag="-2141500130" Tag="85" IconVisible="False" LeftMargin="-11.5000" RightMargin="-11.5000" TopMargin="67.1852" BottomMargin="-21.1852" FontSize="14" LabelText="高级使魔碎片" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="86.0000" Y="17.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="31.5000" Y="-12.6852" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="165" B="0" />
                    <PrePosition X="0.5000" Y="-0.2014" />
                    <PreSize X="1.3651" Y="0.2698" />
                    <FontResource Type="Normal" Path="csb/fonts/fonts2.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="91.5296" Y="135.2900" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2288" Y="0.5412" />
                <PreSize X="0.1575" Y="0.2520" />
                <FileData Type="Normal" Path="ui/lucky/luckygetdi.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_10_0_0" ActionTag="-535533704" Tag="530" IconVisible="False" LeftMargin="167.1950" RightMargin="169.8050" TopMargin="83.2100" BottomMargin="103.7900" LeftEage="29" RightEage="29" TopEage="29" BottomEage="29" Scale9OriginX="29" Scale9OriginY="29" Scale9Width="5" Scale9Height="5" ctype="ImageViewObjectData">
                <Size X="63.0000" Y="63.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="-234339204" Tag="86" IconVisible="False" LeftMargin="2.5000" RightMargin="2.5000" TopMargin="67.1852" BottomMargin="-21.1852" FontSize="14" LabelText="高级宝石" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="58.0000" Y="17.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="31.5000" Y="-12.6852" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="165" B="0" />
                    <PrePosition X="0.5000" Y="-0.2014" />
                    <PreSize X="0.9206" Y="0.2698" />
                    <FontResource Type="Normal" Path="csb/fonts/fonts2.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_16" ActionTag="-1352888014" Tag="306" IconVisible="False" LeftMargin="-11.5000" RightMargin="-11.5000" TopMargin="-8.0000" BottomMargin="-12.0000" LeftEage="28" RightEage="28" TopEage="27" BottomEage="27" Scale9OriginX="28" Scale9OriginY="27" Scale9Width="30" Scale9Height="29" ctype="ImageViewObjectData">
                    <Size X="86.0000" Y="83.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="31.5000" Y="29.5000" />
                    <Scale ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4683" />
                    <PreSize X="1.3651" Y="1.3175" />
                    <FileData Type="Normal" Path="ui/common/drop-19.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="198.6950" Y="135.2900" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4967" Y="0.5412" />
                <PreSize X="0.1575" Y="0.2520" />
                <FileData Type="Normal" Path="ui/lucky/luckygetdi.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_10_0_0_0" ActionTag="707522003" Tag="531" IconVisible="False" LeftMargin="274.3604" RightMargin="62.6396" TopMargin="83.2100" BottomMargin="103.7900" LeftEage="29" RightEage="29" TopEage="29" BottomEage="29" Scale9OriginX="29" Scale9OriginY="29" Scale9Width="5" Scale9Height="5" ctype="ImageViewObjectData">
                <Size X="63.0000" Y="63.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0_0" ActionTag="-2070791386" Tag="87" IconVisible="False" LeftMargin="2.5000" RightMargin="2.5000" TopMargin="67.1852" BottomMargin="-21.1852" FontSize="14" LabelText="大量魂石" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="58.0000" Y="17.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="31.5000" Y="-12.6852" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="165" B="0" />
                    <PrePosition X="0.5000" Y="-0.2014" />
                    <PreSize X="0.9206" Y="0.2698" />
                    <FontResource Type="Normal" Path="csb/fonts/fonts2.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_15_0" ActionTag="848718368" Tag="534" IconVisible="False" LeftMargin="-11.5000" RightMargin="-11.5000" TopMargin="-8.0000" BottomMargin="-12.0000" LeftEage="26" RightEage="26" TopEage="16" BottomEage="16" Scale9OriginX="26" Scale9OriginY="16" Scale9Width="34" Scale9Height="51" ctype="ImageViewObjectData">
                    <Size X="86.0000" Y="83.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="31.5000" Y="29.5000" />
                    <Scale ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4683" />
                    <PreSize X="1.3651" Y="1.3175" />
                    <FileData Type="Normal" Path="ui/common/drop-20.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="305.8604" Y="135.2900" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7647" Y="0.5412" />
                <PreSize X="0.1575" Y="0.2520" />
                <FileData Type="Normal" Path="ui/lucky/luckygetdi.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="ui/common/alldi.jpg" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>