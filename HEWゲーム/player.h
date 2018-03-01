//=============================================================================
//
// �v���C���[���� [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "collision.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER			(4)							// �v���C���[�̐�

#define PLAYER_WIDTH		(100)
#define PLAYER_HEIGHT		(100)
#define HALF_PLAYER_WIDTH	(PLAYER_WIDTH / 2)
#define HALF_PLAYER_HEIGHT	(PLAYER_HEIGHT / 2)
#define HALF_PLAYER_DEPTH	(0.0f)

#define PLAYER_JUMP_SPEED	(15.f)						// �v���C���[�̃W�����v����
#define PLAYER_BIG_JUMP_SPEED (20.f)					// �v���C���[�̑�W�����v����
#define GRAVITY_ACCELARATION (-0.5f)					// �d�͉����x

#define PLAYER_BB_MAX		(D3DXVECTOR3(HALF_PLAYER_WIDTH, HALF_PLAYER_HEIGHT, HALF_PLAYER_DEPTH))// �v���C���[�o�E���f�B���O�{�b�N�X��max���_���W
#define PLAYER_BB_MIN		(-PLAYER_BB_MAX)			// �v���C���[�o�E���f�B���O�{�b�N�X��min���_���W

#define SLIDING_HIT_BOX		{D3DXVECTOR3(-HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, -HALF_PLAYER_DEPTH), D3DXVECTOR3(HALF_PLAYER_WIDTH, 0.0f, HALF_PLAYER_DEPTH)}



//*****************************************************************************
// �񋓑̒�`
//*****************************************************************************
enum PLAYER_STATE {
	PLAYER_NONE,
	PLAYER_ONGROUND,	// ���n
	PLAYER_JUMP,		// �W�����v��
	PLAYER_SLIDING,		// �X���C�f�B���O
	PLAYER_DEAD,		// ���S
	PLAYER_STATEMAX,
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	int lane_no;				// �������郌�[���ԍ�
	int skillpoint;				// �X�L�����p�ׂ̈ɒǉ�
	bool kengen;				// �X�L���𔭓����錠���������Ă��邩�ǂ���
	LPDIRECT3DTEXTURE9 texture; // �e�N�X�`���ǂݍ��ݏꏊ
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;			// ���݂̈ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ���݂̌���
	D3DXVECTOR3 rotDest;		// �ړI�̌���
	D3DXVECTOR3 scl;			// �X�P�[��
	float ground;				// �n�ʂ̍���
	float distance;				// �i�񂾋���
	int life;					// �v���C���[�̗̑�
	PLAYER_STATE state;			// �v���C���[�̏��
	PLAYER_STATE next_state;	// �v���C���[�̎��̏��
	float speed_factor;			// ���x�W��
	float state_counter;		// ��Ԃ̃^�C�}�[
	bool is_invincible;			// ���G��Ԃ��ǂ���
	int invincible_counter;		// ���G���Ԃ̃J�E���g
	BOUNDING_BOX hit_box;		// �v���C���[�̓����蔻��
	BOUNDING_BOX screen_box;	// ��ʊO����p�{�b�N�X
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int no);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);
D3DXVECTOR3 GetMovePlayer(void);
int NumPlayer(void);

#endif
