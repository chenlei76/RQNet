#pragma once
#include "inference_module.h"
#pragma pack(push)
#pragma pack(1)
struct YoloTargetLabel {
	float x;
	float y;
	float w;
	float h;
	float probility;
	float class_ids[1];
};

#pragma pack(pop) 
struct AnchorBoxItem {
	float width;
	float height;
	int masked_index;
};
#include "box.h"
struct ObjectInfo; 

struct TruthInLayer {
	
	int cell_x; // �����ӵ� x����
	int cell_y; // �����ֵ� y����
	float x_offset;
	float y_offset;
	int best_anchor_index; // in layer
	float best_anchor_width;
	float best_anchor_height;

	int best_recall_x;
	int best_recall_y;
	int best_recall_anchor;
	//int cur_pred_x;
	//int cur_pred_y;
	//int cur_anchor_index;

	int class_id;
	int recalls; 
	Box box;
	const ObjectInfo* original;  // �ڱ���ļ��еĿ����Ҫ�������������
	float best_iou;// ����ûʲô�� 
};

class YoloModule : public InferenceModule {
protected:
	int cells_count;
	string mask_anchor_str;
	vector<AnchorBoxItem> masked_anchors; 
	int EntryIndex(int anchor, int loc, int entry); 
	bool Resize(int w, int h);
	bool Detect(); 
	void DeltaClass(float* output, float* delta, int cls_index, int class_id, float* p_class_conf = nullptr);
	float DeltaTruth(const TruthInLayer& truth, float* o, float* d, int cells, RotateType rt, 
		float& object_conf, float& class_conf, int& class_identified);
	bool CalcDelta();
	//bool RescueMissTruth(TruthInLayer& missT, CpuPtr<int>& truth_map, int miss_truth_index, RotateType rt);
public:
	YoloModule(const XMLElement* element, Layer* l, CNNNetwork* network, InferenceModule* prev);
	~YoloModule();
	bool Forward(ForwardContext& context); 
	bool Backward(CudaTensor& delta);
	bool OutputIRModel(ofstream& xml, ofstream& bin, stringstream& edges, size_t& bin_offset, int& l_index) const;
	uint32_t GetFlops() const;
};