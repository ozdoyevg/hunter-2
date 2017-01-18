xof 0303txt 0032
template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}

template FVFData {
 <b6e70a0e-8ef9-4e83-94ad-ecc8b0c04897>
 DWORD dwFVF;
 DWORD nDWords;
 array DWORD data[nDWords];
}

template EffectInstance {
 <e331f7e4-0559-4cc2-8e99-1cec1657928f>
 STRING EffectFilename;
 [...]
}

template EffectParamFloats {
 <3014b9a0-62f5-478c-9b86-e4ac9f4e418b>
 STRING ParamName;
 DWORD nFloats;
 array FLOAT Floats[nFloats];
}

template EffectParamString {
 <1dbc4c88-94c1-46ee-9076-2c28818c9481>
 STRING ParamName;
 STRING Value;
}

template EffectParamDWord {
 <e13963bc-ae51-4c5d-b00f-cfa3a9d97ce5>
 STRING ParamName;
 DWORD Value;
}


Material Material__11_01_-_DefaultSub0 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "../media/textures/weapon.jpg";
 }
}

Material Material__11_02_-_DefaultSub1 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "../media/textures/fire.dds";
 }
}

Frame Box01 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;
 }

 Mesh  {
  4;
  -0.170191;-0.559791;-4.739949;,
  -0.703419;-0.479999;-4.739949;,
  -0.089132;-0.018100;-4.739949;,
  -0.622359;0.061693;-4.739949;;
  2;
  3;2,3,0;,
  3;1,0,3;;

  MeshNormals  {
   1;
   0.000000;0.000000;1.000000;;
   2;
   3;0,0,0;,
   3;0,0,0;;
  }

  MeshMaterialList  {
   2;
   2;
   1,
   1;
   { Material__11_01_-_DefaultSub0 }
   { Material__11_02_-_DefaultSub1 }
  }

  MeshTextureCoords  {
   4;
   0.000500;0.999500;,
   0.999500;0.999500;,
   0.000500;0.000499;,
   0.999500;0.000500;;
  }

  XSkinMeshHeader {
   1;
   0;
   1;
  }

  SkinWeights {
   "Bone04";
   4;
   0,
   1,
   2,
   3;
   1.000000,
   1.000000,
   1.000000,
   1.000000;
   0.000000,0.000000,18.318251,0.000000,0.000000,18.318251,0.000000,0.000000,-18.318251,0.000000,0.000000,0.000000,-81.745491,4.557195,7.343044,1.000000;;
  }
 }
}

Frame Bone04 {
 

 FrameTransformMatrix {
  0.000000,0.000000,-0.054590,0.000000,0.000000,0.054590,0.000000,0.000000,0.054590,0.000000,0.000000,0.000000,-0.276959,-0.259423,-3.060379,1.000000;;
 }

 Frame {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,-0.000000,0.000000,-0.000000,0.000000,1.000000,0.000000,-0.000001,-0.000000,0.000000,1.000000;;
  }
 }
}

AnimationSet Anim-1 {
 

 Animation Anim-Box01 {
  
  { Box01 }

  AnimationOptions {
   1;
   0;
  }

  AnimationKey {
   4;
   31;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   320;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   480;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   640;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   800;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   960;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1120;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1280;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1440;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1600;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1760;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1920;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   2080;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   2240;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   2400;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   2560;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   2720;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   2880;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   3040;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   3200;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   3360;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   3520;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   3680;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   3840;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   4000;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   4160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   4320;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   4480;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   4640;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   4800;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;;
  }
 }

 Animation Anim-Bone04 {
  
  { Bone04 }

  AnimationOptions {
   1;
   0;
  }

  AnimationKey {
   4;
   31;
   0;16;0.000000,0.000000,-0.054590,0.000000,0.000000,0.054590,0.000000,0.000000,0.054590,0.000000,0.000000,0.000000,-0.276959,-0.259423,-3.060379,1.000000;;,
   160;16;0.000000,0.000000,-0.054590,0.000000,0.000000,0.052962,-0.000000,0.000000,0.052962,-0.000000,-0.000000,0.000000,-0.276959,-0.178537,-2.960045,1.000000;;,
   320;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.002492,0.000000,0.000000,0.002492,-0.000000,0.000000,0.000000,-0.329517,-0.149189,-2.849921,1.000000;;,
   480;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.159057,-2.835457,1.000000;;,
   640;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.289734,-0.070707,1.000000;;,
   800;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   960;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1120;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1280;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1440;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1600;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1760;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1920;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   2080;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   2240;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   2400;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   2560;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   2720;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   2880;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   3040;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   3200;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   3360;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   3520;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   3680;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   3840;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   4000;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   4160;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   4320;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   4480;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   4640;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   4800;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;;
  }
 }
}

AnimationSet Anim-2 {
 

 Animation Anim-Box01 {
  
  { Box01 }

  AnimationOptions {
   1;
   0;
  }

  AnimationKey {
   4;
   11;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   320;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   480;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   640;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   800;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   960;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1120;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1280;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1440;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;,
   1600;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.134485,0.000000,-0.053308,1.000000;;;
  }
 }

 Animation Anim-Bone04 {
  
  { Bone04 }

  AnimationOptions {
   1;
   0;
  }

  AnimationKey {
   4;
   11;
   0;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   160;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   320;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   480;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   640;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   800;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   960;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1120;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1280;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1440;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;,
   1600;16;-0.000000,-0.000000,-0.054590,0.000000,0.000000,0.001281,0.000000,0.000000,0.001281,-0.000000,0.000000,0.000000,-0.325451,-0.412959,1.595257,1.000000;;;
  }
 }
}