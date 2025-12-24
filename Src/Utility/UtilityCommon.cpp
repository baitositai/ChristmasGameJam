#include "UtilityCommon.h"
#include "../Manager/Common/SceneManager.h"

int UtilityCommon::Round(float v)
{
    return static_cast<int>(roundf(v));
}

std::vector<std::string> UtilityCommon::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;

    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double UtilityCommon::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float UtilityCommon::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int UtilityCommon::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double UtilityCommon::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float UtilityCommon::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int UtilityCommon::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

double UtilityCommon::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double UtilityCommon::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

int UtilityCommon::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > DX_PI_F)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -DX_PI_F)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int UtilityCommon::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > 180.0f)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -180.0f)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int UtilityCommon::Lerp(int start, int end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    int ret = start;
    ret += Round(t * static_cast<float>(end - start));
    return ret;
}

float UtilityCommon::Lerp(float start, float end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    float ret = start;
    ret += t * (end - start);
    return ret;
}

double UtilityCommon::Lerp(double start, double end, double t)
{
    // 線形補間
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

Vector2 UtilityCommon::Lerp(const Vector2& start, const Vector2& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    Vector2 ret = start;
    ret.x += Round(t * static_cast<float>((end.x - start.x)));
    ret.y += Round(t * static_cast<float>((end.y - start.y)));
    return ret;
}

VECTOR UtilityCommon::Lerp(const VECTOR& start, const VECTOR& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    VECTOR ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double UtilityCommon::LerpDeg(double start, double end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

COLOR_F UtilityCommon::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;
}

Vector2 UtilityCommon::Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t)
{
    Vector2 a = Lerp(p1, p2, t);
    Vector2 b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR UtilityCommon::Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t)
{
    VECTOR a = Lerp(p1, p2, t);
    VECTOR b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

bool UtilityCommon::IsTimeOver(float& totalTime, const float& waitTime)
{
    //デルタタイム
    auto delta = SceneManager::GetInstance().GetDeltaTime();
    totalTime += delta;

    //待機時間を超過しているか判断
    if (totalTime >= waitTime)
    {
        return true;
    }

    return false;
}

int UtilityCommon::WrapIndex(int index, int max)
{
    return (index % max + max) % max;
}

int UtilityCommon::WrapStepIndex(const int index, const int step, const int min, const int max)
{
    // 範囲サイズ
    int range = (max - min);

    // 加減算してオフセットで0基準に調整
    int value = ((index - min + step) % range + range) % range + min;

    return value;
}

float UtilityCommon::EaseInQuad(const float _time, const float _totalTime, const float _start, const float _end)
{
    float distance = _end - _start;
    float t = _time / _totalTime;
    return distance * t * t + _start;
}

float UtilityCommon::EaseOutQuad(const float _time, const float _totalTime, const float _start, const float _end)
{
    float distance = _end - _start;
    float t = _time / _totalTime;
    return -distance * _time * (_time - 2) + _start;
}

float UtilityCommon::EaseInOutBack(float _time, const float _totalTime, const float _start, const float _end)
{
    // 補間定数1
    constexpr float C1 = 1.70158f;

    // 補間定数2（オーバーシュート調整用）
    constexpr float C2 = C1 * 1.525f;

    // 開始から終了までの距離
    float distance = _end - _start;

    // 補間係数
    float t = _time / _totalTime;

    // Clamp t to [0, 1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // 補間の実行（EaseInOutBack）
    float easedT;
    if (t < 0.5f)
    {
        float twoT = 2.0f * t;
        easedT = (twoT * twoT * ((C2 + 1.0f) * twoT - C2)) / 2.0f;
    }
    else
    {
        float twoTMinus2 = 2.0f * t - 2.0f;
        easedT = (twoTMinus2 * twoTMinus2 * ((C2 + 1.0f) * twoTMinus2 + C2) + 2.0f) / 2.0f;
    }

    // 値の補間
    return distance * easedT + _start;
}

std::vector<std::vector<int>> UtilityCommon::LoadCSVData(const std::wstring& filePath)
{
    std::vector<std::vector<int>> csvData;

    //ファイルの準備
    std::wifstream ifs = std::wifstream(filePath);
    // ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // UTF-8対応
    if (!ifs)
    {
        OutputDebugString(L"ステージのifstream準備失敗\n");
        return csvData;
    }

    std::wstring line;

    // CSVデータの読み込み
    while (getline(ifs, line))
    {
        std::wstringstream lineStream(line);
        std::wstring cell;
        std::vector<int> row;

        // 各セルをカンマで区切って取得し、数値に変換して行に追加
        while (getline(lineStream, cell, L',')) {
            try
            {
                row.push_back(std::stoi(cell));
            }
            catch (...)
            {
                row.push_back(0); // 数値変換失敗時は0を入れる
            }
        }
        csvData.push_back(row); // 行を追加
    }

    ifs.close();

    return csvData;
}

std::wstring UtilityCommon::GetWStringFromString(const std::string& str)
{
    //根幹数を正しく使うには2回呼び出す必要がある
    std::wstring ret = L"";
    //最初の呼び出しは返される文字列の文字列数
    auto length = MultiByteToWideChar(
        CP_ACP,									//現在のコードページ
        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,	//エラーしたら情報を返す
        str.c_str(),							//元の文字列へのポインタ
        str.length(),							//元の文字列の長さ
        nullptr,								//変換先の文字列のバッファ
        0										//最後の引数をnullptr,0にすることで
    );											//wstringに必要な文字列数を返している

    if (length <= 0)
    {
        return L"";
    }
    ret.resize(length);	//必要なメモリを確保

    //二回目の呼び出しは実際の文字列を得る
    MultiByteToWideChar(
        CP_ACP,									// 文字コード
        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
        str.c_str(),							// 変換元の文字列
        str.length(),							// 得られたワイド文字列を入れるアドレス
        ret.data(),								// 得られたワイド文字列を入れるメモリサイズ
        ret.size()								// 変換先の文字列のバッファのサイズ
    );

    return ret;
}

std::string UtilityCommon::ConvertUtf8ToSjis(const std::string& utf8_str)
{
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wstr[0], wlen);

    int sjlen = WideCharToMultiByte(932, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string sjis(sjlen, '\0');
    WideCharToMultiByte(932, 0, wstr.c_str(), -1, &sjis[0], sjlen, nullptr, nullptr);
    return sjis;
}

float UtilityCommon::NormalizeMinMax(const float value, const float min, const float max)
{
    if (max == min)
    {
        return 0.0; // 分母が0になる場合は0を返すなどのエラー処理
    }
    double normalized = (value - min) / (max - min);

    // 0から1の範囲にクランプ（収める）する
    if (normalized < 0.0) return 0.0;
    if (normalized > 1.0) return 1.0;
    return normalized;
}
