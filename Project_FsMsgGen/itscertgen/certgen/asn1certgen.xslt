<?xml version="1.0"?>
<!-- 
     ****************************************************************************************
     *   This scripts generates XER certificate representation from given profile.          *
     *   Use ETSI asn1certgen converter to convert XER to COER and sign certificates.       *
     *   see https://oldforge.etsi.org/svn/ITS/branches/STF525/tools/itscertgen/asn1certgen *
     ****************************************************************************************
-->      
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://exslt.org/dates-and-times"
                extension-element-prefixes="date"                
                >
 <xsl:variable name="base-time"       select="'2022-01-01'"/>
 <xsl:variable name="local-region"    select="250"/>
 <xsl:variable name="base-latitude"   select="436169490.0"/>
 <xsl:variable name="base-longitude"  select="70533080.0"/>
<!--
 <xsl:variable name="base-latitude"   select="0"/>
 <xsl:variable name="base-longitude"  select="0"/>
-->
 <xsl:variable name="koeff-latitude"  select="89.833458"/>  <!--tenth of microseconds per metter-->
 <xsl:variable name="koeff-longitude" select="89.833458"/>  <!--tenth of microseconds per metter-->

 <xsl:variable name="AID-CAM"     select="36"/>
 <xsl:variable name="AID-DENM"    select="37"/>
 <xsl:variable name="AID-SPAT"    select="137"/>
 <xsl:variable name="AID-MAP"     select="138"/>
 <xsl:variable name="AID-IVI"     select="139"/>
 <xsl:variable name="AID-SREM"    select="140"/>
 <xsl:variable name="AID-SSEM"    select="637"/>
 <xsl:variable name="AID-GN-MGMT" select="141"/>
 <xsl:variable name="AID-CRT-REQ" select="623"/>
 <xsl:variable name="AID-CRL"     select="622"/>
 <xsl:variable name="AID-CTL"     select="624"/>
 <xsl:variable name="AID-GPC"     select="540802"/>
 <xsl:variable name="AID-CPM"     select="639"/>
 <xsl:variable name="AID-VRU"     select="638"/>
 <xsl:variable name="AID-SA"      select="540801"/>

 <xsl:template match = "/">
  <xsl:apply-templates select="certificate"/>
 </xsl:template>
 
 <xsl:template match = "certificate">
  <EtsiTs103097Certificate>
    <xsl:apply-templates select="version"/>
    <xsl:apply-templates select="." mode="cert_type"/>
    <xsl:apply-templates select="signer"/>
    <xsl:apply-templates select="subject"/>
    <xsl:apply-templates select="signature"/>
  </EtsiTs103097Certificate>
 </xsl:template>

 <xsl:template match = "version">
<!--   <version><xsl:value-of select = "." /></version> -->
   <version>3</version>
 </xsl:template>
 
  <xsl:template match="certificate" mode="cert_type">
    <type><xsl:choose>
      <xsl:when test="@type='implicit'"><implicit/></xsl:when>
      <xsl:when test="@type='explicit'"><explicit/></xsl:when>
      <xsl:when test="subject/attribute[@type='verification_key']/public_key"><explicit/></xsl:when>
      <xsl:when test="subject/attribute[@type='verification_key']/reconstruction_value"><implicit/></xsl:when>
      <xsl:otherwise><explicit/></xsl:otherwise>
    </xsl:choose></type>
  </xsl:template>

 <xsl:template match = "signer">
  <xsl:choose>
  <xsl:when test="@type='digest'">
    <issuer>
     <xsl:attribute name="name">
       <xsl:value-of select="@name" />
     </xsl:attribute>
     <xsl:comment><xsl:value-of select = "@name" /></xsl:comment>
     <xsl:choose>
      <xsl:when test="contains(@alg, '384')">
        <sha384AndDigest/>
      </xsl:when>
      <xsl:otherwise>
        <sha256AndDigest/>
      </xsl:otherwise>
     </xsl:choose>
    </issuer>
    </xsl:when>
    <xsl:when test="@type='self'">
     <issuer>
       <self><sha256/></self>
     </issuer>
    </xsl:when>
    </xsl:choose>
 </xsl:template>

 <xsl:template mode="sha256Digest" match = "@name">
   <!-- TODO: should calculate sha256 of external certificate -->
   <xsl:value-of select = "." />
 </xsl:template>
 
 <xsl:template mode="sha384Digest" match = "@name">
   <!-- TODO: should calculate sha384 of external certificate -->
   <xsl:value-of select = "." />
 </xsl:template>

 <xsl:template match = "subject">

  <xsl:variable name="defaultCrtReqAppSSP">
   <xsl:choose>
    <xsl:when test="@type = 'ROOT'">0</xsl:when>
    <xsl:when test="@type = 'AA'">00110010</xsl:when>
    <xsl:when test="@type = 'TLM'">0</xsl:when>
    <xsl:when test="@type = 'EA'">00001110</xsl:when>
    <xsl:when test="@type = 'EC'">11000000</xsl:when>
    <xsl:when test="@type = 'AT'">0</xsl:when>
    <xsl:otherwise>0</xsl:otherwise>
   </xsl:choose>
  </xsl:variable>

  <xsl:variable name="defaultCrtReqIssueSSP">
   <xsl:choose>
    <xsl:when test="@type = 'ROOT'">11111110</xsl:when>
    <xsl:when test="@type = 'AA'">0</xsl:when>
    <xsl:when test="@type = 'TLM'">0</xsl:when>
    <xsl:when test="@type = 'EA'">11000000</xsl:when>
    <xsl:when test="@type = 'EC'">0</xsl:when>
    <xsl:when test="@type = 'AT'">0</xsl:when>
    <xsl:otherwise>0</xsl:otherwise>
   </xsl:choose>
  </xsl:variable>

  <toBeSigned>
   <xsl:choose>
    <xsl:when test="@name = ''">
     <id><none></none></id>
    </xsl:when>
    <xsl:otherwise>
     <id><name><xsl:value-of select="@name" /></name></id>
    </xsl:otherwise>
   </xsl:choose>
   <cracaId>00 00 00</cracaId>
   <crlSeries>0</crlSeries>

   <xsl:apply-templates select="../validity/restriction[@type='time']"/>
   <xsl:apply-templates mode="region" select="../validity/restriction[@type='region']"/>
   <xsl:apply-templates select="attribute[@type='assurance_level']"/>
   <xsl:apply-templates select="attribute[@type='its_aid_ssp_list']"/>
   <xsl:if test="attribute[@type='its_aid_list']">
    <certIssuePermissions>
     <xsl:apply-templates select="attribute[@type='its_aid_list']">
      <xsl:with-param name="certType" select="@type"/>
     </xsl:apply-templates>
    </certIssuePermissions>
   </xsl:if>
   <xsl:apply-templates select="attribute[@type='encryption_key']"/>
   <xsl:apply-templates select="attribute[@type='verification_key']"/>
  </toBeSigned>
 </xsl:template>
 
 <xsl:template match="attribute[@type='assurance_level']">
  <xsl:variable name="level" select="assurance/@level"/>
  <xsl:variable name="confidence">
    <xsl:choose>
      <xsl:when test="assurance/@confidence">
        <xsl:value-of select="assurance/@confidence" />
      </xsl:when>
      <xsl:otherwise>
       <xsl:text>0</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <assuranceLevel>
    <xsl:call-template name="ConvertDecToHex">
      <xsl:with-param name="index" select="$level * 32 + $confidence"/>
    </xsl:call-template>
  </assuranceLevel>
  <xsl:comment>level=<xsl:value-of select="$level"/> confidence=<xsl:value-of select="$confidence"/> </xsl:comment>
 </xsl:template>
 
 <xsl:template match="attribute[@type='its_aid_list']">
  <xsl:param name="certType"/>
   <PsidGroupPermissions>
    <subjectPermissions>
     <explicit>
      <xsl:apply-templates mode="certIssuePermissions" select="aid">
       <xsl:with-param name="certType" select="$certType"/>
      </xsl:apply-templates>
     </explicit>
    </subjectPermissions>
    <minChainLength>
     <xsl:choose>
      <xsl:when test="@minChainLength"><xsl:value-of select="@minChainLength"/></xsl:when>
      <xsl:when test="$certType = 'ROOT'">2</xsl:when>
      <xsl:otherwise>1</xsl:otherwise>
     </xsl:choose>
    </minChainLength>
    <chainLengthRange>
     <xsl:choose>
      <xsl:when test="$certType = 'ROOT'">0</xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
     </xsl:choose>
    </chainLengthRange>
    <eeType>
     <xsl:choose>
      <xsl:when test="@eeType = 'enrol' or @eeType = '40'">
        <xsl:text>01000000</xsl:text>
      </xsl:when>
      <xsl:when test="@eeType = 'app' or @eeType = '80'">
        <xsl:text>10000000</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>11000000</xsl:text>
      </xsl:otherwise>
     </xsl:choose>
    </eeType>
   </PsidGroupPermissions>
 </xsl:template>
 
 <xsl:template mode="certIssuePermissions" match="aid">
  <xsl:param name="certType"/>
  <xsl:variable name="SSPValue">
   <xsl:choose>
    <xsl:when test="normalize-space(.) != ''">
     <xsl:choose>
      <xsl:when test="contains(normalize-space(.), '/')">
        <xsl:value-of select="substring-before(normalize-space(.),'/')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="normalize-space(.)"/>
      </xsl:otherwise>
     </xsl:choose>
    </xsl:when>
    <xsl:when test="@value = 'CAM' or @value=$AID-CAM">01 FF FC</xsl:when>
    <xsl:when test="@value = 'DENM' or @value=$AID-DENM">01 FF FF FF</xsl:when>
    <xsl:when test="@value = 'TLM' or @value = 'SPAT' or @value=$AID-SPAT">01 E0</xsl:when>
    <xsl:when test="@value = 'RLT' or @value = 'MAP' or @value=$AID-MAP">01 C0</xsl:when>
    <xsl:when test="@value = 'IVI' or @value=$AID-IVI">01 00 00 00 FF F8</xsl:when>
    <xsl:when test="@value = 'TLC' or @value=$AID-SREM or @value='SREM'">02 FF FF E0</xsl:when>
    <xsl:when test="@value = 'SSEM' or @value=$AID-SSEM">01</xsl:when>
    <xsl:when test="@value = 'GPC' or @value=$AID-GPC">01</xsl:when>
    <xsl:when test="@value = 'CRT-REQ' or @value = $AID-CRT-REQ">
     <xsl:choose>
      <xsl:when test="$certType = 'ROOT'">01 FE</xsl:when>
      <xsl:when test="$certType = 'EA'">01 C0</xsl:when>
      <xsl:otherwise>01 00</xsl:otherwise>
     </xsl:choose>
    </xsl:when>
   </xsl:choose>
  </xsl:variable>
  <xsl:variable name="SSPBitmask">
   <xsl:choose>
    <xsl:when test="normalize-space(.) != ''">
     <xsl:choose>
      <xsl:when test="contains(normalize-space(.), '/')">
        <xsl:value-of select="substring-after(normalize-space(.),'/')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="translate(normalize-space(.),'0123456789ABCDEF','FFFFFFFFFFFFFFFF')"/>
      </xsl:otherwise>
     </xsl:choose>
    </xsl:when>
    <xsl:when test="@value = 'CAM' or @value=$AID-CAM">FF 00 03</xsl:when>
    <xsl:when test="@value = 'DENM' or @value=$AID-DENM">FF 00 00 00</xsl:when>
    <xsl:when test="@value = 'TLM' or @value = 'SPAT' or @value=$AID-SPAT">FF 1F</xsl:when>
    <xsl:when test="@value = 'RLT' or @value = 'MAP' or @value=$AID-MAP">FF 3F</xsl:when>
    <xsl:when test="@value = 'IVI' or @value=$AID-IVI">FF 00 00 00 00 07</xsl:when>
    <xsl:when test="@value = 'TLC' or @value=$AID-SREM or @value='SREM'">FF 00 00 1F</xsl:when>
    <xsl:when test="@value = 'SSEM' or @value=$AID-SSEM">FF</xsl:when>
    <xsl:when test="@value = 'GPC' or @value=$AID-GPC">FF</xsl:when>
    <xsl:when test="@value = 'CRT-REQ' or @value = $AID-CRT-REQ">
     <xsl:choose>
      <xsl:when test="$certType = 'ROOT'">FF 01</xsl:when>
      <xsl:when test="$certType = 'EA'">FF FF</xsl:when>
      <xsl:otherwise>01 FF</xsl:otherwise>
     </xsl:choose>
    </xsl:when>
   </xsl:choose>
  </xsl:variable>
  <PsidSspRange>
   <xsl:comment><xsl:value-of select="@value"/></xsl:comment>
   <psid>
    <xsl:choose>
     <xsl:when test="@value = 'CAM' "><xsl:value-of select="$AID-CAM"/></xsl:when>
     <xsl:when test="@value = 'DENM' "><xsl:value-of select="$AID-DENM"/></xsl:when>
     <xsl:when test="@value = 'TLM' or @value = 'SPAT'"><xsl:value-of select="$AID-SPAT"/></xsl:when>
     <xsl:when test="@value = 'RLT' or @value = 'MAP'"><xsl:value-of select="$AID-MAP"/></xsl:when>
     <xsl:when test="@value = 'IVI' "><xsl:value-of select="$AID-IVI"/></xsl:when>
     <xsl:when test="@value = 'TLC' or @value = 'SREM'"><xsl:value-of select="$AID-SREM"/></xsl:when>
     <xsl:when test="@value = 'SSEM'"><xsl:value-of select="$AID-SSEM"/></xsl:when>
     <xsl:when test="@value = 'GPC'"><xsl:value-of select="$AID-GPC"/></xsl:when>
     <xsl:when test="@value = 'CRT-REQ'"><xsl:value-of select="$AID-CRT-REQ"/></xsl:when>
     <xsl:when test="@value = 'CRL'"><xsl:value-of select="$AID-CRL"/></xsl:when>
     <xsl:when test="@value = 'CTL'"><xsl:value-of select="$AID-CTL"/></xsl:when>
     <xsl:when test="@value = 'GN-MGMT'"><xsl:value-of select="$AID-GN-MGMT"/></xsl:when>
     <xsl:otherwise><xsl:value-of select="@value"/></xsl:otherwise>
    </xsl:choose>
   </psid>
   <xsl:if test="string-length($SSPValue) &gt; 0">
    <sspRange>
     <bitmapSspRange>
      <sspValue><xsl:value-of select="$SSPValue"/></sspValue>
      <sspBitmask><xsl:value-of select="$SSPBitmask"/></sspBitmask>
     </bitmapSspRange>
    </sspRange>
   </xsl:if>
  </PsidSspRange>
 </xsl:template>

 <xsl:template match="attribute[@type='its_aid_ssp_list']">
   <appPermissions>
    <xsl:apply-templates mode="appPermissions" select="ssp"/>
   </appPermissions>
 </xsl:template>

 <xsl:template mode="appPermissions" match="ssp">
   <PsidSsp>
    <xsl:comment><xsl:value-of select="@aid"/></xsl:comment>
    <psid>
     <xsl:choose>
      <xsl:when test="@aid = 'CAM' "><xsl:value-of select="$AID-CAM"/></xsl:when>
      <xsl:when test="@aid = 'DENM' "><xsl:value-of select="$AID-DENM"/></xsl:when>
      <xsl:when test="@aid = 'TLM' or @aid = 'SPAT'"><xsl:value-of select="$AID-SPAT"/></xsl:when>
      <xsl:when test="@aid = 'RLT' or @aid = 'MAP'"><xsl:value-of select="$AID-MAP"/></xsl:when>
      <xsl:when test="@aid = 'IVI' "><xsl:value-of select="$AID-IVI"/></xsl:when>
      <xsl:when test="@aid = 'TLC' or @aid = 'SREM'"><xsl:value-of select="$AID-SREM"/></xsl:when>
      <xsl:when test="@aid = 'SSEM'"><xsl:value-of select="$AID-SSEM"/></xsl:when>
      <xsl:when test="@aid = 'GPC'"><xsl:value-of select="$AID-GPC"/></xsl:when>
      <xsl:when test="@aid = 'CRT-REQ'"><xsl:value-of select="$AID-CRT-REQ"/></xsl:when>
      <xsl:when test="@aid = 'CRL'"><xsl:value-of select="$AID-CRL"/></xsl:when>
      <xsl:when test="@aid = 'CTL'"><xsl:value-of select="$AID-CTL"/></xsl:when>
      <xsl:when test="@aid = 'GN-MGMT'"><xsl:value-of select="$AID-GN-MGMT"/></xsl:when>
      <xsl:otherwise><xsl:value-of select="@aid"/></xsl:otherwise>
     </xsl:choose>
    </psid>
    <xsl:if test="string-length(.) &gt; 0">
     <ssp><bitmapSsp><xsl:value-of select="." /></bitmapSsp></ssp>
    </xsl:if>
   </PsidSsp>
 </xsl:template>

 <xsl:template match="restriction[@type='time']">
   <validityPeriod>
     <xsl:apply-templates mode="validityperiod" select="@start"/>
     <xsl:apply-templates mode="validityperiod" select="@end">
      <xsl:with-param name="start" select="@start"/>
     </xsl:apply-templates>
     <xsl:apply-templates mode="validityperiod" select="@duration">
      <xsl:with-param name="start" select="@start"/>
     </xsl:apply-templates>
   </validityPeriod>
 </xsl:template>

 <xsl:template mode="validityperiod" match="@start">
   <xsl:variable name="start">
     <xsl:call-template name="time-value">
       <xsl:with-param name="value" select = "." /> 
       <xsl:with-param name="base"  select = "$base-time" /> 
     </xsl:call-template>
   </xsl:variable>
   <start>
     <xsl:comment><xsl:value-of select="date:add('2004-01-01', concat('PT',$start,'S'))"/></xsl:comment>
     <xsl:value-of select="$start"/>
   </start>
 </xsl:template>

 <xsl:template name="time-value">
   <xsl:param name = "value" />
   <xsl:param name = "base" />
   <xsl:choose>
     <xsl:when test="starts-with($value,'+')">
       <xsl:comment> <xsl:value-of select="concat('P',translate(substring($value,2),'ymd','YMD'))"/> </xsl:comment>
       <xsl:value-of select="date:seconds(date:add($base, 
                             concat('P',translate(substring($value,2),'ymd','YMD'))
                             )) - date:seconds('2004-01-01')"/>
     </xsl:when>
     <xsl:when test="starts-with($value,'-')">
       <xsl:comment> <xsl:value-of select="concat('-P',translate(substring($value,2),'ymd','YMD'))"/></xsl:comment>
       <xsl:value-of select="date:seconds(date:add($base, 
                             concat('-P',translate(substring($value,2),'ymd','YMD'))
                             )) - date:seconds('2004-01-01')"/>
     </xsl:when>
     <xsl:when test="$value = 'cur'">
       <xsl:value-of select="date:seconds(date:date()) - date:seconds('2004-01-01')"/>
     </xsl:when>
     <xsl:otherwise>
       <xsl:value-of select="date:seconds($value) - date:seconds('2004-01-01')"/>
     </xsl:otherwise>
   </xsl:choose>
 </xsl:template>


 <xsl:template mode="validityperiod" match="@end">
   <xsl:param name = "start" />
   <xsl:variable name="vstart">
    <xsl:call-template name="time-value">
      <xsl:with-param name="value" select="$start"/>
      <xsl:with-param name="base"  select="$base-time"/>
    </xsl:call-template>
   </xsl:variable>
   <xsl:variable name="vend">
    <xsl:call-template name="time-value">
      <xsl:with-param name="value" select="."/>
      <xsl:with-param name="base"  select="$base-time"/>
    </xsl:call-template>
   </xsl:variable>

   <duration><hours>
     <xsl:comment><xsl:value-of select="date:add('2004-01-01', concat('PT',$vend,'S'))"/></xsl:comment>
     <xsl:value-of select="($vend - $vstart) div 3600"/>
   </hours></duration>
 </xsl:template>

 <xsl:template mode="validityperiod" match="@duration">
   <xsl:param name = "start" />
   <xsl:variable name="duration">
    <xsl:call-template name="time-value">
      <xsl:with-param name="value" select="."/>
      <xsl:with-param name="base"  select="0"/>
    </xsl:call-template>
   </xsl:variable>
   <duration><hours>
     <xsl:comment><xsl:value-of select="date:add('2004-01-01', concat('PT',$start + $duration,'S'))"/></xsl:comment>
     <xsl:value-of select="$duration div 3600"/>
   </hours></duration>
 </xsl:template>

 <xsl:template mode="region" match="none">
 </xsl:template>

 <xsl:template mode="region" match="circle">
  <region>
    <circularRegion>
      <center>
        <latitude>
         <xsl:call-template name="geo-value">
           <xsl:with-param name="base"  select="$base-latitude"/>
           <xsl:with-param name="koeff" select="$koeff-latitude"/>
           <xsl:with-param name="value" select="@latitude"/>
         </xsl:call-template>
        </latitude>
        <longitude>
         <xsl:call-template name="geo-value">
           <xsl:with-param name="base"  select="$base-longitude"/>
           <xsl:with-param name="koeff" select="$koeff-longitude"/>
           <xsl:with-param name="value" select="@longitude"/>
         </xsl:call-template>
        </longitude>
      </center>
      <radius><xsl:value-of select="@radius"/></radius>
    </circularRegion>
  </region>
 </xsl:template>

 <xsl:template mode="region" match="rectangle">
  <xsl:variable name="nw" select="location[position() mod 2 = 1]" />
  <xsl:variable name="se" select="location[position() mod 2 = 0]" />
  <region>
    <rectangularRegion>
      <xsl:for-each select="$nw">
        <xsl:call-template name="rectangular-region">
          <xsl:with-param name="nw" select="$nw"/>
          <xsl:with-param name="se" select="$se"/>
          <xsl:with-param name="pos" select="position()"/>
        </xsl:call-template>
      </xsl:for-each>
    </rectangularRegion>
  </region>
 </xsl:template>

 <xsl:template name="rectangular-region">
   <xsl:param name="nw"/>
   <xsl:param name="se"/>
   <xsl:param name="pos"/>
   <RectangularRegion>
     <northWest>
       <latitude>
         <xsl:call-template name="geo-value">
           <xsl:with-param name="base"  select="$base-latitude"/>
           <xsl:with-param name="koeff" select="$koeff-latitude"/>
           <xsl:with-param name="value" select="$nw[$pos]/@latitude"/>
         </xsl:call-template>
       </latitude>
       <longitude>
         <xsl:call-template name="geo-value">
           <xsl:with-param name="base"  select="$base-longitude"/>
           <xsl:with-param name="koeff" select="$koeff-longitude"/>
           <xsl:with-param name="value" select="$nw[$pos]/@longitude"/>
         </xsl:call-template>
       </longitude>
     </northWest>
     <southEast>
       <latitude>
         <xsl:call-template name="geo-value">
           <xsl:with-param name="base"  select="$base-latitude"/>
           <xsl:with-param name="koeff" select="$koeff-latitude"/>
           <xsl:with-param name="value" select="$se[$pos]/@latitude"/>
         </xsl:call-template>
       </latitude>
       <longitude>
         <xsl:call-template name="geo-value">
           <xsl:with-param name="base"  select="$base-longitude"/>
           <xsl:with-param name="koeff" select="$koeff-longitude"/>
           <xsl:with-param name="value" select="$se[$pos]/@longitude"/>
         </xsl:call-template>
       </longitude>
     </southEast>
   </RectangularRegion>
 </xsl:template>

 <xsl:template mode="region" match="polygon">
  <region>
    <polygonalRegion>
      <xsl:for-each select="location">
        <TwoDLocation>
         <latitude>
           <xsl:call-template name="geo-value">
             <xsl:with-param name="base"  select="$base-latitude"/>
             <xsl:with-param name="koeff" select="$koeff-latitude"/>
             <xsl:with-param name="value" select="@latitude"/>
           </xsl:call-template>
         </latitude>
         <longitude>
           <xsl:call-template name="geo-value">
             <xsl:with-param name="base"  select="$base-longitude"/>
             <xsl:with-param name="koeff" select="$koeff-longitude"/>
             <xsl:with-param name="value" select="@longitude"/>
           </xsl:call-template>
         </longitude>
        </TwoDLocation>
      </xsl:for-each>
    </polygonalRegion>
  </region>
 </xsl:template>

 <xsl:template mode="region" match="restriction[@type='region' and id]">
   <region>
    <identifiedRegion>
     <xsl:apply-templates mode="region" select="id"/>
    </identifiedRegion>
   </region>
 </xsl:template>

 <xsl:template mode="region" match="id">
   <xsl:choose>
    <xsl:when test="@local != '0'">
      <countryAndRegions>
        <xsl:call-template name="country-only">
          <xsl:with-param name="id" select="@id"/>
        </xsl:call-template>
        <regions>
          <INTEGER>@local</INTEGER>
        </regions>
      </countryAndRegions>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="country-only">
        <xsl:with-param name="id" select="@id"/>
      </xsl:call-template>
    </xsl:otherwise>
   </xsl:choose>
 </xsl:template>

 <xsl:template name="geo-value">
   <xsl:param    name="base"/>
   <xsl:param    name="value"/>
   <xsl:param    name="koeff"/>
   <xsl:variable name="km" select="substring-before"/>
   <xsl:choose>
     <xsl:when test="starts-with($value,'-')">
      <xsl:call-template name="geo-value">
        <xsl:with-param name="base"  select="$base"/>
        <xsl:with-param name="koeff" select="-1 * $koeff"/>
        <xsl:with-param name="value" select="substring($value,2)"/>
      </xsl:call-template>
     </xsl:when>
     <xsl:when test="starts-with($value,'+')">
      <xsl:call-template name="geo-value">
        <xsl:with-param name="base"  select="$base"/>
        <xsl:with-param name="koeff" select="$koeff"/>
        <xsl:with-param name="value" select="substring($value,2)"/>
      </xsl:call-template>
     </xsl:when>
     <xsl:otherwise>
       <xsl:choose>
         <xsl:when test="substring($value,string-length($value)-1) = 'km'">
           <xsl:value-of select="floor($base + ( $koeff * 1000 * substring($value,1,string-length($value)-2) ))"/>
         </xsl:when>
         <xsl:when test="substring($value,string-length($value)) = 'm'">
           <xsl:value-of select="floor($base + ( $koeff * substring($value,1,string-length($value)-1) ))"/>
         </xsl:when>
        <xsl:otherwise>
           <xsl:value-of select="floor($base + ( 10000000.0 * $value))"/>
        </xsl:otherwise>
       </xsl:choose>
     </xsl:otherwise>
   </xsl:choose>
 </xsl:template>


 <xsl:template name="country-only">
   <xsl:param name="id"/>
     <xsl:choose>
       <xsl:when test="$id='RGN_LOCAL'">
         <xsl:call-template name="country-only">
          <xsl:with-param name="id" select="$local-region"/>
         </xsl:call-template>
       </xsl:when>
       <xsl:otherwise>
         <countryOnly><xsl:value-of select="$id"/></countryOnly>
       </xsl:otherwise>
     </xsl:choose>
 </xsl:template>

 <xsl:template match="attribute[@type='encryption_key']">
  <encryptionKey>
   <xsl:apply-templates select="public_key"  mode="encryption_key"/>
  </encryptionKey>
 </xsl:template>

 <xsl:template match="attribute[@type='verification_key']">
 <verifyKeyIndicator>
  <xsl:if test="public_key">
    <verificationKey>
     <xsl:apply-templates select="public_key"  mode="verification_key"/>
    </verificationKey>
  </xsl:if>
  <xsl:if test="reconstruction_value">
    <reconstructionValue>
     <xsl:apply-templates select="reconstruction_value/ecc_point"/>
    </reconstructionValue>
  </xsl:if>
 </verifyKeyIndicator>
 </xsl:template>
 
 <xsl:template match="public_key" mode="verification_key">
  <xsl:choose>
   <xsl:when test="contains(@algorithm, 'brainpool') and contains(@algorithm, '256')">
    <ecdsaBrainpoolP256r1>
     <xsl:apply-templates select="ecc_point"/>
    </ecdsaBrainpoolP256r1>
   </xsl:when>
   <xsl:when test="contains(@algorithm, 'brainpool') and contains(@algorithm, '384')">
    <ecdsaBrainpoolP384r1>
     <xsl:apply-templates select="ecc_point">
      <xsl:with-param name="algorithm" select="'384'"/>
     </xsl:apply-templates>
    </ecdsaBrainpoolP384r1>
   </xsl:when>
   <xsl:otherwise>
    <ecdsaNistP256>
     <xsl:apply-templates select="ecc_point"/>
    </ecdsaNistP256>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>

 <!--xsl:template match="reconstruction_value">
 </xsl:template-->

 <xsl:template match="public_key" mode="encryption_key">
  <supportedSymmAlg>
   <xsl:choose>
    <xsl:when test="@sym_alg != 'aes_128_ccm' and @sym_alg != 0">
    </xsl:when>
    <xsl:otherwise>
     <aes128Ccm/>
    </xsl:otherwise>
   </xsl:choose>
  </supportedSymmAlg>
  <publicKey>
   <xsl:choose>
    <xsl:when test="@algorithm != 'ecies_nistp256' and @algorithm != 0">
    </xsl:when>
    <xsl:otherwise>
     <eciesNistP256>
      <xsl:apply-templates select="ecc_point"/>
     </eciesNistP256>
    </xsl:otherwise>
   </xsl:choose>
  </publicKey>
 </xsl:template>

 <xsl:template match="ecc_point">
  <xsl:param name="algorithm" select="'256'"/>
  <xsl:choose>
   <xsl:when test="@type = 'x_coordinate_only' or @type = 0">
    <x-only>
     <xsl:value-of select="."/>
    </x-only>
   </xsl:when>
   <xsl:when test="@type = 'compressed' or @type = 'compressed_y0' or @type = 2">
    <compressed-y-0>
     <xsl:value-of select="."/>
    </compressed-y-0>
   </xsl:when>
   <xsl:when test="@type = 'compressed_y1' or @type = 3">
    <compressed-y-1>
     <xsl:value-of select="."/>
    </compressed-y-1>
   </xsl:when>
   <xsl:when test="@type = 'uncompressed' or @type = 4">
    <xsl:element name="{concat('uncompressedP', $algorithm)}">
     <x><xsl:value-of select="x"/></x>
     <y><xsl:value-of select="y"/></y>
    </xsl:element>
   </xsl:when>
  </xsl:choose>
 </xsl:template>
 
 <xsl:template match = "signature">
   <signature>
     <xsl:choose>
       <xsl:when test="contains(@algorithm, 'brainpool') and contains(@algorithm, '256')">
         <ecdsaBrainpoolP256r1Signature>
           <xsl:call-template name="signature-point">
             <xsl:with-param name="algorithm" select="@algorithm"/>
           </xsl:call-template> 
         </ecdsaBrainpoolP256r1Signature>
       </xsl:when>
       <xsl:when test="contains(@algorithm, 'brainpool') and contains(@algorithm, '384')">
         <ecdsaBrainpoolP384r1Signature>
           <xsl:call-template name="signature-point">
             <xsl:with-param name="algorithm" select="@algorithm"/>
           </xsl:call-template> 
         </ecdsaBrainpoolP384r1Signature>
       </xsl:when>
       <xsl:otherwise>
         <ecdsaNistP256Signature>
           <xsl:call-template name="signature-point">
             <xsl:with-param name="algorithm" select="@algorithm"/>
           </xsl:call-template> 
         </ecdsaNistP256Signature>
       </xsl:otherwise>
     </xsl:choose>
   </signature>
 </xsl:template>

 <xsl:template name = "signature-point">
   <rSig>
     <xsl:choose>
       <xsl:when test="ecc_point">
         <xsl:apply-templates match="ecc_point"/>
       </xsl:when>
       <xsl:otherwise>
         <x-only/>
       </xsl:otherwise>
     </xsl:choose>
   </rSig>
   <sSig/>
  </xsl:template>


  <xsl:template name="ConvertDecToHex">
    <xsl:param name="index" />
    <xsl:if test="$index > 0">
      <xsl:call-template name="ConvertDecToHex">
        <xsl:with-param name="index" select="floor($index div 16)" />
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="$index mod 16 &lt; 10">
          <xsl:value-of select="$index mod 16" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <xsl:when test="$index mod 16 = 10">A</xsl:when>
            <xsl:when test="$index mod 16 = 11">B</xsl:when>
            <xsl:when test="$index mod 16 = 12">C</xsl:when>
            <xsl:when test="$index mod 16 = 13">D</xsl:when>
            <xsl:when test="$index mod 16 = 14">E</xsl:when>
            <xsl:when test="$index mod 16 = 15">F</xsl:when>
            <xsl:otherwise>A</xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
