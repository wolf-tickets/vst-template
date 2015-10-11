#include "PluginTemplate.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum EParams {
  kGain = 0,
  kNumParams
};

enum ELayout {
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 100,
  kGainY = 100,
  kKnobFrames = 60
};

PluginTemplate::PluginTemplate(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), gain_(1.) {

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGain)->InitDouble("Gain", 50., 0., 100.0, 0.01, "%");
  GetParam(kGain)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_BLACK);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));

  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

PluginTemplate::~PluginTemplate() {}

void PluginTemplate::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames) {
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];

  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
    *out1 = *in1 * gain_;
    *out2 = *in2 * gain_;
  }
}

void PluginTemplate::Reset() {
  IMutexLock lock(this);
}

void PluginTemplate::OnParamChange(int paramIdx) {
  IMutexLock lock(this);

  switch (paramIdx) {
    case kGain:
      gain_ = GetParam(kGain)->Value() / 100.;
      break;
    default:
      break;
  }
}
