<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:param name="profile"/>
<xsl:output method="text" />
<xsl:template match = "/">
<xsl:variable name="v" select="certificate/signer/@name"/>
<xsl:value-of select="concat('$(outdir)/', $profile, '.oer : ')"/><xsl:if test="$v"><xsl:value-of select="concat('$(outdir)/', $v , '.oer')"/></xsl:if>
</xsl:template>
</xsl:stylesheet>