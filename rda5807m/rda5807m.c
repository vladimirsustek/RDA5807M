#include "rda5807m.h"

#define RDS_USED            1

#define swapbytes(byte) ((((byte) & 0x00FF) << 8) | (((byte) & 0xFF00) >> 8))

static uint16_t RDA5807mGetReg0x0A(void) {

    uint16_t RDA5807MReg = 0;

    twi_readFromSlave(RDA5807M_I2C_ADR, (uint8_t*)&RDA5807MReg, RDA5807M_REG_SIZE);
    RDA5807MReg = swapbytes(RDA5807MReg);

    printf("0x0A: 0x%04x\n", RDA5807MReg);

    return RDA5807MReg;
}

static uint16_t RDA5807mGetReg0x0B(void) {

    uint16_t RDA5807MReg[2] = {0};

    twi_readFromSlave(RDA5807M_I2C_ADR, (uint8_t*)&RDA5807MReg, RDA5807M_REG_SIZE*2);
    RDA5807MReg[1] = swapbytes(RDA5807MReg[1]);

    printf("0x0B: 0x%04x\n", RDA5807MReg[1]);

    return RDA5807MReg[1];
}

uint8_t RDA5807mMute(uint8_t mute) {

    uint16_t RDA5807Register;
    /* For Mute only 1st register needed to be accessed */
#if RDS_USED
    RDA5807Register = (1 << DHIZ) | (1 << BASS) | (1 << RCLK_DIR_MODE) | (1 << RDS_EN) | (1 << ENABLE);
#else
    RDA5807Register = (1 << DHIZ) | (1 << BASS) | (1 << RCLK_DIR_MODE) | (1 << ENABLE);
#endif
    if (!mute) {
        RDA5807Register |= (1 << DMUTE);
    }

    RDA5807Register = swapbytes(RDA5807Register);

    twi_writeToSlave(RDA5807M_I2C_ADR, (uint8_t*)&RDA5807Register, RDA5807M_REG_SIZE);

    return RDA5807mFN_OK;

}

uint8_t RDA5807mReset(void) {

    /* For Mute only 1st register needed to be accessed */

    uint16_t RDA5807Register = {0};
	/* Register REG_ADR_02 */
	RDA5807Register = (1 << SOFT_RESET);

    RDA5807Register = swapbytes(RDA5807Register);

    twi_writeToSlave(RDA5807M_I2C_ADR, (uint8_t*)&RDA5807Register, RDA5807M_REG_SIZE);

    return RDA5807mFN_OK;

}

uint8_t RDA5807mInit(void) {

    /* For complete initialization, 6 (out of 7) registers must be written */
    uint16_t RDA5807Registers[6] = {0};
	/* Register REG_ADR_02 */
#if RDS_USED
    RDA5807Registers[0] = (1 << DHIZ) | (1 << BASS) | (1 << RCLK_DIR_MODE) | (1 << RDS_EN) | (1 << ENABLE);
#else
    RDA5807Registers[0] = (1 << DHIZ) | (1 << BASS) | (1 << RCLK_DIR_MODE) | (1 << ENABLE);
#endif
	/* Register REG_ADR_03 */
	RDA5807Registers[1] = (1 << BAND_1) ;
	/* Register REG_ADR_04 */
#if RDS_USED
	RDA5807Registers[2] = (1 << RDS_FIFO_EN) | (1 << RDS_FIFO_CLR);
#else
    RDA5807Registers[2]= 0u;
#endif
	/* Register REG_ADR_05 */
	RDA5807Registers[3] = (1 << INT_MODE) | (1 << SEEK_MODE_1)  | (1 << SEEKTH_1)| (1 << SEEKTH_0) | (1 << LNA_PORT_SEL_1) | (1 << LNA_ICSEL_1) | (1 << VOLUME_0);
    /* Register REG_ADR_06 */
	RDA5807Registers[4] = (1 << OPEN_MODE_1) | (1 << OPEN_MODE_0);
    /* Register REG_ADR_07 */
	RDA5807Registers[5] = (1 << TH_SOFRBLEND_4) | (1 << MODE_65M_50M) | (1 << SEEK_TH_OLD_2) | (1 << SOFTBLEND_EN);

    for(uint8_t idx = 0; idx < 6; idx++) {

        RDA5807Registers[idx] = swapbytes(RDA5807Registers[idx]);
    }

    twi_writeToSlave(RDA5807M_I2C_ADR, (uint8_t*)RDA5807Registers, RDA5807M_REG_SIZE*6);

	return RDA5807mFN_OK;

}

uint8_t RDA5807mSetFreq(uint16_t freq) {

    /* For frequency change 2nd register must be accessed */
    uint16_t RDA5807Registers[2] = {0};

    if (RDA5807mWW_FREQ_MIN > freq ||
        RDA5807mWW_FREQ_MAX < freq)
            return RDA5807mFN_ERR;
#if RDS_USED
    RDA5807Registers[0] = (1 << DHIZ)| (1 << DMUTE) | (1 << BASS)| (1 << RCLK_DIR_MODE) | (1 << RDS_EN) | (1 << ENABLE);
#else
    RDA5807Registers[0] = (1 << DHIZ)| (1 << DMUTE) | (1 << BASS)| (1 << RCLK_DIR_MODE)| (1 << ENABLE);
#endif
    freq -= 760;
    freq <<= 6;
    RDA5807Registers[1] = freq | (1 << TUNE) | (1 << BAND_1);

    RDA5807Registers[0] = swapbytes(RDA5807Registers[0]);
    RDA5807Registers[1] = swapbytes(RDA5807Registers[1]);

    twi_writeToSlave(RDA5807M_I2C_ADR, (uint8_t*)RDA5807Registers, 4);

    return RDA5807mFN_OK;
}

uint8_t RDA5807mSetVolume(uint8_t volume) {

    if (volume > RDA5807mVOLUME_MAX) return RDA5807mFN_ERR;

    /* To set volume, 4th register must be accessed,
       so 4x16b write transaction is needed */
    uint16_t RDA5807Registers[4];
	/* Register REG_ADR_02 */
#if RDS_USED
    RDA5807Registers[0] = (1 << DHIZ)| (1 << DMUTE) | (1 << BASS)| (1 << RCLK_DIR_MODE) | (1 << RDS_EN) | (1 << ENABLE);
#else
    RDA5807Registers[0] = (1 << DHIZ)| (1 << DMUTE) | (1 << BASS)| (1 << RCLK_DIR_MODE)| (1 << ENABLE);
#endif	/* Register REG_ADR_03 */
	RDA5807Registers[1] = 0 ;
	/* Register REG_ADR_04 */

#if RDS_USED
	RDA5807Registers[2] = (1 << RDS_FIFO_EN) | (1 << RDS_FIFO_CLR);
#else
    RDA5807Registers[2] = 0u;
#endif
	/* Register REG_ADR_05 */
	RDA5807Registers[3] = (1 << INT_MODE) | (1 << SEEK_MODE_1)  | (1 << SEEKTH_1)| (1 << SEEKTH_0) | (1 << LNA_PORT_SEL_1) | (1 << LNA_ICSEL_1) | volume;

    for(uint8_t idx = 0; idx < 4; idx++) RDA5807Registers[idx] = swapbytes(RDA5807Registers[idx]);

    twi_writeToSlave(RDA5807M_I2C_ADR, (uint8_t*)RDA5807Registers, 8);

    return RDA5807mFN_OK;
}

uint8_t RDA5807mGetRSSI(void) {

    uint16_t RDA5807MReg;

    RDA5807MReg = RDA5807mGetReg0x0B();

    /* Masking only RSSI bits 15:9 */
    RDA5807MReg &= ((1 << RSSI_6) | (1 << RSSI_5) | (1 << RSSI_4) | (1 << RSSI_3) | (1 << RSSI_2) | (1 << RSSI_1) | (1 << RSSI_0));

    /* Shifting value to be within 6:0 */
    RDA5807MReg >>= RSSI_0;

    return (uint8_t)RDA5807MReg;
}

uint8_t RDA5807mGetRDSR(void) {

    uint16_t RDA5807MReg = RDA5807mGetReg0x0A();

    return ((RDA5807MReg & (1 << RDSR)) >> (RDSR));
}

uint8_t RDA5807mGetRDSS(void) {

    uint16_t RDA5807MReg = RDA5807mGetReg0x0A();

    return ((RDA5807MReg & (1 << RDSS)) >> (RDSS));
}

uint8_t RDA5807misChannelStereo(void) {

    uint16_t RDA5807MReg = RDA5807mGetReg0x0A();

    return ((RDA5807MReg & (1 << ST)) >> (ST));
}

uint8_t RDA5807mIsChannelStation(void) {

    uint16_t RDA5807MReg = RDA5807mGetReg0x0B();

    return ((RDA5807MReg & (1 << ST)) >> (ST));
}

uint16_t RDA5807mGetRDSBlockA(void) {

    uint8_t Registers[6] = {0};
    uint16_t RDA5807MReg;

    twi_readFromSlave(RDA5807M_I2C_ADR, Registers, 6);
    RDA5807MReg = ((Registers[4] << 8 )| Registers[5]);

    return RDA5807MReg;
}

uint16_t RDA5807mGetRDSBlockB(void) {

    uint8_t Registers[8] = {0};
    uint16_t RDA5807MReg;

    twi_readFromSlave(RDA5807M_I2C_ADR, Registers, 8);
    RDA5807MReg = ((Registers[6] << 8 )| Registers[7]);

    return RDA5807MReg;
}

uint16_t RDA5807mGetRDSBlockC(void) {

    uint8_t Registers[10] = {0};
    uint16_t RDA5807MReg;

    twi_readFromSlave(RDA5807M_I2C_ADR, Registers, 10);
    RDA5807MReg = ((Registers[8] << 8 )| Registers[9]);

    return RDA5807MReg;
}

uint16_t RDA5807mGetRDSBlockD(void) {

    uint8_t Registers[12] = {0};
    uint16_t RDA5807MReg;

    twi_readFromSlave(RDA5807M_I2C_ADR, Registers, 12);
    RDA5807MReg = ((Registers[10] << 8 )| Registers[11]);

    return RDA5807MReg;
}

uint8_t RDA5807mGetErrBlockA(void) {

    uint16_t RDA5807MReg;

	RDA5807MReg = RDA5807mGetReg0x0B();

    RDA5807MReg &= (1 << BLERA_1) | (1 << BLERA_0);
    RDA5807MReg >>= BLERA_0;

    return (uint8_t)RDA5807MReg;
}

uint8_t RDA5807mGetErrBlockB(void) {

    uint16_t RDA5807MReg;

	RDA5807MReg = RDA5807mGetReg0x0B();

    RDA5807MReg &= (1 << BLERB_1) | (1 << BLERB_0);
    RDA5807MReg >>= BLERB_0;

    return (uint8_t)RDA5807MReg;
}


uint8_t RDA5807mClearRDSFIFO(void) {

    uint16_t RDA5807Registers[3] = {0};
	/* Register REG_ADR_02 */
#if RDS_USED
    RDA5807Registers[0] = (1 << DHIZ)| (1 << DMUTE) | (1 << BASS)| (1 << RCLK_DIR_MODE) | (1 << RDS_EN) | (1 << ENABLE);
#else
    RDA5807Registers[0] = (1 << DHIZ)| (1 << DMUTE) | (1 << BASS)| (1 << RCLK_DIR_MODE)| (1 << ENABLE);
#endif	/* Register REG_ADR_03 */
	RDA5807Registers[1] = 0 ;
	/* Register REG_ADR_04 */

#if RDS_USED
	RDA5807Registers[2] = (1 << RDS_FIFO_EN) | (1 << RDS_FIFO_CLR);
#endif
    for(uint8_t idx = 0; idx < 3; idx++) RDA5807Registers[idx] = swapbytes(RDA5807Registers[idx]);

    twi_writeToSlave(RDA5807M_I2C_ADR, (uint8_t*)RDA5807Registers, 6);

    return RDA5807mFN_OK;

}
