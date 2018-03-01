//=============================================================================
//
// �X�e�[�W���� [stage.h]
// Author : 
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "collision.h"
#include "game.h"
#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LANE_DATA	(512)

//*****************************************************************************
// �񋓑̐錾
//*****************************************************************************
enum STAGE {
	STAGE_PLAIN   = 0,		// �����X�e�[�W
	STAGE_DESERT  = 1,		// �����X�e�[�W
	STAGE_VOLCANO = 2,		// �ΎR�X�e�[�W
	STAGE_SNOW    = 3,		// ��R�X�e�[�W
	STAGE_MAX     = 4,
};

enum OBSTACLE_TYPE {
	OBSTACLE_JUMP,			// �W�����v�p��Q��
	OBSTACLE_LARGE_JUMP,	// ��W�����v�p��Q��
	OBSTACLE_SLIDING,		// �X���C�f�B���O�p��Q��
	OBSTACLE_MAX,
};

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	bool use;			// �v���C���[���Q�����Ă����true
	float speed_factor;	// ���x�W�� �f�t�H���g��1.0f
	float lane_length;	// ���[��1�T���̒���
} LANE;

typedef struct {
	int length;
	char tile[MAX_LANE_DATA];
} LANE_DATA;

typedef struct {
	int lane_no;					// �������郌�[���̔ԍ�
	STAGE stage;					// ��������X�e�[�W
	OBSTACLE_TYPE obstacle_type;	// ��Q���̃^�C�v
	LPDIRECT3DTEXTURE9 texture;		// �e�N�X�`���ǂݍ��ݏꏊ
	LPDIRECT3DVERTEXBUFFER9 vtx;	// ���_�o�b�t�@
	BOUNDING_BOX hit_box;			// �����蔻��p�o�E���f�B���O�{�b�N�X
	BOUNDING_BOX screen_box;		// ��ʊO����p�o�E���f�B���O�{�b�N�X
	D3DXVECTOR3 pos;				// ���݂̈ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ���݂̌���
	D3DXVECTOR3 rotDest;			// �ړI�̌���
} OBSTACLE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);
OBSTACLE *GetObstacle(void);
LANE *GetLane(int no);

#endif