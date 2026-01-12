#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <cmath>
#include <array>

namespace BrokenArrow {
namespace Math {

class Noise {
public:
    static float Perlin(float x, float y, float z) {
        static const std::array<int, 512> perm = GeneratePermutation();

        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        float u = Fade(x);
        float v = Fade(y);
        float w = Fade(z);

        int A = perm[X] + Y;
        int AA = perm[A] + Z;
        int AB = perm[A + 1] + Z;
        int B = perm[X + 1] + Y;
        int BA = perm[B] + Z;
        int BB = perm[B + 1] + Z;

        float res = Lerp(w, 
            Lerp(v, 
                Lerp(u, Grad(perm[AA], x, y, z), Grad(perm[BA], x-1, y, z)),
                Lerp(u, Grad(perm[AB], x, y-1, z), Grad(perm[BB], x-1, y-1, z))),
            Lerp(v, 
                Lerp(u, Grad(perm[AA+1], x, y, z-1), Grad(perm[BA+1], x-1, y, z-1)),
                Lerp(u, Grad(perm[AB+1], x, y-1, z-1), Grad(perm[BB+1], x-1, y-1, z-1))));
        
        return (res + 1.0f) / 2.0f;
    }

    static float Perlin(const Vector3& pos) {
        return Perlin(pos.x, pos.y, pos.z);
    }

    static float Simplex(float x, float y, float z) {
        const float F3 = 1.0f / 3.0f;
        const float G3 = 1.0f / 6.0f;

        float s = (x + y + z) * F3;
        int i = static_cast<int>(std::floor(x + s));
        int j = static_cast<int>(std::floor(y + s));
        int k = static_cast<int>(std::floor(z + s));

        float t = float(i + j + k) * G3;
        float X0 = i - t;
        float Y0 = j - t;
        float Z0 = k - t;
        float x0 = x - X0;
        float y0 = y - Y0;
        float z0 = z - Z0;

        int i1, j1, k1;
        int i2, j2, k2;

        if(x0 >= y0) {
            if(y0 >= z0) { i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; }
            else if(x0 >= z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; }
            else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; }
        } else {
            if(y0 < z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; }
            else if(x0 < z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; }
            else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; }
        }

        float x1 = x0 - i1 + G3;
        float y1 = y0 - j1 + G3;
        float z1 = z0 - k1 + G3;
        float x2 = x0 - i2 + 2.0f * G3;
        float y2 = y0 - j2 + 2.0f * G3;
        float z2 = z0 - k2 + 2.0f * G3;
        float x3 = x0 - 1.0f + 3.0f * G3;
        float y3 = y0 - 1.0f + 3.0f * G3;
        float z3 = z0 - 1.0f + 3.0f * G3;

        static const std::array<int, 512> perm = GeneratePermutation();

        int ii = i & 255;
        int jj = j & 255;
        int kk = k & 255;

        int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
        int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
        int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
        int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

        float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
        float n0 = (t0 < 0.0f) ? 0.0f : (t0 * t0 * t0 * t0 * GradSimplex(gi0, x0, y0, z0));

        float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
        float n1 = (t1 < 0.0f) ? 0.0f : (t1 * t1 * t1 * t1 * GradSimplex(gi1, x1, y1, z1));

        float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
        float n2 = (t2 < 0.0f) ? 0.0f : (t2 * t2 * t2 * t2 * GradSimplex(gi2, x2, y2, z2));

        float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
        float n3 = (t3 < 0.0f) ? 0.0f : (t3 * t3 * t3 * t3 * GradSimplex(gi3, x3, y3, z3));

        return 32.0f * (n0 + n1 + n2 + n3);
    }

    static float Simplex(const Vector3& pos) {
        return Simplex(pos.x, pos.y, pos.z);
    }

    static float FractalNoise(const Vector3& pos, int octaves = 4, float lacunarity = 2.0f, float persistence = 0.5f) {
        float total = 0.0f;
        float frequency = 1.0f;
        float amplitude = 1.0f;
        float maxValue = 0.0f;

        for (int i = 0; i < octaves; i++) {
            total += Perlin(pos * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }

        return total / maxValue;
    }

private:
    static float Fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    static float Grad(int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    static float GradSimplex(int hash, float x, float y, float z) {
        static const std::array<Vector3, 12> gradients = {
            Vector3(1,1,0), Vector3(-1,1,0), Vector3(1,-1,0), Vector3(-1,-1,0),
            Vector3(1,0,1), Vector3(-1,0,1), Vector3(1,0,-1), Vector3(-1,0,-1),
            Vector3(0,1,1), Vector3(0,-1,1), Vector3(0,1,-1), Vector3(0,-1,-1)
        };
        const Vector3& g = gradients[hash % 12];
        return g.x * x + g.y * y + g.z * z;
    }

    static std::array<int, 512> GeneratePermutation() {
        std::array<int, 256> p = {
            151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
            8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
            35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
            134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
            55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
            18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
            250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
            189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
            172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
            228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
            107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };

        std::array<int, 512> perm;
        for (int i = 0; i < 256; i++) {
            perm[i] = p[i];
            perm[256 + i] = p[i];
        }
        return perm;
    }
};

} // namespace Math
} // namespace BrokenArrow
