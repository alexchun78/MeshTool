#pragma once
class CMeshSimplificationManager
{
    public:
        CMeshSimplificationManager(const HMODULE handle, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
        //CMeshSimplificationManager(const HMODULE handle);
        ~CMeshSimplificationManager();

    public:
        bool DoSimplificationQE(IN const float reductionRate, IN const float agressive);
        void GetSimplificationOutputData(OUT std::vector<MeshIOLib::Vertex>& verts, OUT std::vector<MeshIOLib::Triangle>& tris);

    private:
        MeshSimplificationQELib::CSimplificationQuadricError* m_CSimplificationQE;
        HMODULE m_handle;
};

