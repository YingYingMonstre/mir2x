#pragma once

#include "mir2xmap.hpp"

#include <string>
#include "wilimagepackage.hpp"
#include <cstdint>
#include <functional>
#include <vector>
#include <Fl/Fl_Shared_Image.H>

#pragma pack(push, 1)
// this is abandoned
typedef struct{
    int32_t     bIsLight;
    char        cLightSizeType;
    char        cLightColorType;
}LIGHTINFO;

typedef struct{
    char        szDesc[20];
    uint16_t    wAttr;
    int16_t     shWidth;
    int16_t     shHeight;
    char        cEventFileIndex;
    char        cFogColor;
}MAPFILEHEADER;

typedef struct{
    uint8_t     bFileIndex;
    uint16_t    wTileIndex;
}TILEINFO;

typedef struct{
    uint8_t     bFlag;
    uint8_t     bObj1Ani;
    uint8_t     bObj2Ani;
    uint16_t    wFileIndex;
    uint16_t    wObj1;
    uint16_t    wObj2;
    uint16_t    bDoorIndex;
    uint8_t     bDoorOffset;
    uint16_t    wLigntNEvent;
}CELLINFO;

#pragma pack(pop)

class Mir2Map
{
    private:
        bool            m_Valid;
    private:
        int             m_DoorCount;
        int             m_LightCount;
        int             m_AlphaBlend;
    private:
        uint32_t        m_dwAniSaveTime[8];
        uint8_t         m_bAniTileFrame[8][16];

    private:
        WilImagePackage *m_pxTileImage;

    private:
        TILEINFO       *m_pstTileInfo;
        CELLINFO       *m_pstCellInfo;
        MAPFILEHEADER   m_stMapFileHeader;

    private:
        Mir2xMap        g_Mir2xMap;

    public:
        Mir2Map();
        ~Mir2Map();

    public:
        bool LoadMap(const char *);
        void LoadMapImage(WilImagePackage *);

    public:

        void ExtractOneBaseTile(std::function<void(uint32_t *, uint32_t, uint32_t, int, int, int, int)>, int, int);
        void ExtractOneObjectTile(std::function<void(uint32_t *, uint32_t, uint32_t, int, int, int, int)>, int, int);

        void ExtractBaseTile(std::function<bool(uint32_t, uint32_t)>, std::function<void(uint32_t *, uint32_t, uint32_t, int, int, int, int)>);
        void ExtractObjectTile(std::function<bool(uint32_t, uint32_t)>, std::function<void(uint32_t *, uint32_t, uint32_t, int, int, int, int)>);
        void ExtractGroundInfo(std::function<void(uint32_t, int, int, int)>);

    public:
        void     SetAniTileFrame(int);
        uint32_t GetDoorImageIndex(int, int);

    public:
        // use this API set to extract needed information only
        int  Width();
        int  Height();
        bool Valid();

    public:
        bool Expand(int, int);
        bool CropSize(int, int, int, int);
    public:
        const TILEINFO &BaseTileInfo(int, int);
        const CELLINFO &CellInfo(int, int);
    public:
        void DrawBaseTile(int, int, int, int, std::function<void(uint32_t, uint32_t, int, int)>);
        void DrawObjectTile(int, int, int, int, std::function<bool(uint32_t, uint32_t, Fl_Shared_Image * &, int, int)>, std::function<void(uint32_t, uint32_t, Fl_Shared_Image *, int, int)>);

    public:
        void CompressGroundInfo(std::vector<bool> &, std::vector<uint8_t> &);
        void CompressBaseTileInfo(std::vector<bool> &, std::vector<uint32_t> &);
        void CompressBaseTileInfoPreOrder(int, int, int, std::vector<bool> &, std::vector<uint32_t> &);
        bool EmptyBaseTileBlock(int, int, int);

    public:
        void CompressCellTileInfo(std::vector<bool> &, std::vector<CELLDESC> &);
        void CompressCellTileInfoPreOrder(int, int, int, std::vector<bool> &, std::vector<CELLDESC> &);
        bool EmptyCellTileBlock(int, int, int);

    public:
        bool NewLoadMap(const char *);

    private:
        std::vector<std::vector<std::array<bool, 4>>> m_SelectedGrid;

    private:
        uint32_t    BitPickOne(uint32_t *, uint32_t);


    private:
        bool LoadGroundInfo(uint32_t *, uint32_t, uint32_t *, uint32_t);
        void SetOneGroundInfoGrid(int, int, int, uint32_t);
        void SetGroundInfoBlock(int, int, int, uint32_t);
        void ParseGroundInfoStream(int, int, int, uint32_t *, uint32_t &, uint32_t *, uint32_t &);

    private:
        bool LoadCellDesc(uint32_t *, uint32_t, CELLDESC *, uint32_t);
        void SetCellDescBlock(int, int, int, const CELLDESC &);
        void ParseCellDescStream(int, int, int, uint32_t *, uint32_t &, CELLDESC *, uint32_t &);

    private:
        void SetBaseTileBlock(int, int, int, uint32_t);
        bool LoadBaseTileInfo(uint32_t *, uint32_t, uint32_t *, uint32_t);
        void ParseBaseTileStream(int, int, int, uint32_t *, uint32_t &, uint32_t *, uint32_t &);

    public:
        void Optimize();
        void OptimizeBaseTile(int, int);
        void OptimizeCell(int, int);

    private:
        void SetMapInfo();

    public:
        std::string MapInfo();

    public:
        // work on door
        uint8_t GetDoor(int, int);
        void OpenDoor(int, int, uint8_t);
        void CloseDoor(int, int, uint8_t);
        void OpenAllDoor();
        void CloseAllDoor();
};
