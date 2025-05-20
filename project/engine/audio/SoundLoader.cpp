#include "SoundLoader.h"
#include <cassert>

using namespace Microsoft::WRL;

// 静的メンバ変数の定義
std::unique_ptr<SoundLoader> SoundLoader::instance = nullptr;

// シングルトンインスタンスの取得
SoundLoader* SoundLoader::GetInstance() {
    if (!instance) {
        instance = std::make_unique<SoundLoader>();
    }
    return instance.get();
}

// 終了
void SoundLoader::Finalize() {
    GetIXAudio2();
    instance.reset();
}

void SoundLoader::Initialize() {
    HRESULT result;
    // XAudioエンジンのインスタンスを生成
    result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    // マスターボイスの生成
    result = xAudio2->CreateMasteringVoice(&masterVoice);
}

SoundData SoundLoader::SoundLoadWave(const char* filename)
{
    // ①ファイルオープン
    // ファイル入力ストリームのインスタンス
    std::ifstream file;
    // .wavファイルをバイナリモードで開く
    file.open(filename, std::ios_base::binary);
    // ファイルオープン失敗を検出する
    assert(file.is_open());
    // ②.wavデータ読み込み
    // RIFFヘッダーの読み込み
    RiffHeader riff;
    file.read((char*)&riff, sizeof(riff));
    // ファイルがRIFFかチェック
    if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
        assert(0);
    }
    // タイプがWAVEかチェック
    if (strncmp(riff.type, "WAVE", 4) != 0) {
        assert(0);
    }
    // Formatチャンクの読み込み
    FormatChunk format = {};
    // チャンクヘッダーの確認
    file.read((char*)&format, sizeof(ChunkHeader));
    if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
        assert(0);
    }
    // チャンク本体の読み込み
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char*)&format.fmt, format.chunk.size);
    // Dataチャンクの読み込み
    ChunkHeader data;
    file.read((char*)&data, sizeof(data));
    // JUNKチャンクを検出した場合
    if (strncmp(data.id, "JUNK", 4) == 0) {
        // 読み取り位置をJUNKチャンクの終わりまで進める
        file.seekg(data.size, std::ios_base::cur);
        // 再読み込み
        file.read((char*)&data, sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }
    // Dataチャンクのデータ部(波形データ)の読み込み
    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);
    // Waveファイルを閉じる
    file.close();

    // ④読み込んだ音声データをreturn
    SoundData soundData = {};
    soundData.wfex = format.fmt;
    soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
    soundData.bufferSize = data.size;
    return soundData;
}
