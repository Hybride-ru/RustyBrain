/****************************************************************************
**
** Copyright (C) 2012 Eremin Yakov and Putintsev Roman.
** All rights reserved.
**
**
** This file is part of the RustyBrain project.
**
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
**     RustyBrain is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY;
**
**
****************************************************************************/
#include "rb.voipclient.h"

//  ----------------------------------------------------------------------------
//  VOIP CLIENT CLASS
//  ----------------------------------------------------------------------------

//  PRIVATE
//  ----------------------------------------------------------------------------

void RBVoipClient::createAudioInput()
{
    input = new QAudioInput( infoInput, format );

    input->setNotifyInterval( RB_VOIP_TIMEOUT );
    connect( input,
             SIGNAL(notify()),
             SLOT(slotCapture()) );
    devInput = input->start();
}

void RBVoipClient::createAudioOutput()
{
    output = new QAudioOutput( infoOutput, format );

    output = new QAudioOutput( format, this );
    devOutput = output->start();
}

//  PROTECTED SLOTS
//  ----------------------------------------------------------------------------

void RBVoipClient::slotInputDeviceChanged(int inIndex)
{
    input->stop();
    input->disconnect(this);
    delete input;
    delete devInput;

    infoInput = ui->cbInput->itemData(inIndex).value<QAudioDeviceInfo>();

    createAudioInput();
}

void RBVoipClient::slotOutputDeviceChanged(int inIndex)
{
    output->stop();
    output->disconnect(this);
    delete output;
    //delete devOutput;

    infoOutput = ui->cbOutput->itemData(inIndex).value<QAudioDeviceInfo>();

    createAudioOutput();
}

void RBVoipClient::slotRecive(char *inBuf, int inSize, RNSocket inSock)
{
    devOutput->write( inBuf, inSize );
    free(inBuf);
}

void RBVoipClient::slotCapture()
{
    int size = devInput->read( buffer, RB_VOIP_BUFFER_SIZE );
    if( size > 0 )
        voipService->sendAll( buffer, size );
}

//  PUBLIC
//  ----------------------------------------------------------------------------

RBVoipClient::operator RNService*()
{
    return voipService;
}

RBVoipClient::RBVoipClient(Ui::RBChatClient *inUi)
    : infoInput(QAudioDeviceInfo::defaultInputDevice())
    , infoOutput(QAudioDeviceInfo::defaultOutputDevice())
{
    // ui
    ui = inUi;

    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for(int i = 0; i < devices.size(); ++i)
        ui->cbInput->addItem(devices.at(i).deviceName(), qVariantFromValue(devices.at(i)));
    connect( ui->cbInput,
             SIGNAL(currentIndexChanged(int)),
             SLOT(slotInputDeviceChanged(int)) );

    devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(int i = 0; i < devices.size(); ++i)
        ui->cbOutput->addItem(devices.at(i).deviceName(), qVariantFromValue(devices.at(i)));
    connect( ui->cbOutput,
             SIGNAL(currentIndexChanged(int)),
             SLOT(slotOutputDeviceChanged(int)) );

    // Service
    voipService = new RNService(RB_VOIP_ID);

    connect( voipService,
             SIGNAL(recive(char*,int,RNSocket)),
             SLOT(slotRecive(char*,int,RNSocket)) );

    // Audio
    format.setFrequency(RB_FREQ);
    format.setChannels(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);

    if( !infoInput.isFormatSupported(format) )
    {
        RCLog << RCLog.warning("RBVoipClient") << "Default input format not supported try to use nearest" << RCLog.end();
        format = infoInput.nearestFormat(format);
    }
    createAudioInput();

    if( !infoOutput.isFormatSupported(format) )
    {
        RCLog << RCLog.warning("RBVoipClient") << "Default output format not supported try to use nearest" << RCLog.end();
        format = infoOutput.nearestFormat(format);
    }
    createAudioOutput();
}

RBVoipClient::~RBVoipClient()
{
    delete voipService;

    input->stop();
    output->stop();

    delete input;
    delete output;
}

