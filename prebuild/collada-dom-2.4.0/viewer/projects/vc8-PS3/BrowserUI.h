/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef BROWSERUI_H
#define BROWSERUI_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

#define MAX_PATH	1024
#define MOVE_SPACE 0.5
#define THUMBNAIL_SIZE 0.25
#define THUMBNAIL_HALFSIZE 0.125
#define HDD_DEFAULT_IMAGE "/dev_hdd0/game/NPXX00001/USRDIR/COLLADA/default.raw"
#define APP_HOME_DEFAULT_IMAGE "/app_home/default.raw"
#define API_ERROR(x) {if(x!=CELL_FS_SUCCEEDED){printf("error=0x%x\n",x);}}

enum
{
	DEVICE_MS,
	DEVICE_USB0,
	DEVICE_USB1,
	DEVICE_USB2,
	DEVICE_USB3,
	DEVICE_USB4,
	DEVICE_USB5,
	DEVICE_USB6,
	DEVICE_MAX,
};

class Thumbnail
{
public:
	Thumbnail();
	virtual ~Thumbnail();
	void SetPosition(float x, float y, float z);
	void MovePosition(float x, float y, float z);

	const char * GetDocument() {return documentfile;};
	const char * GetImage() {return imagefile;};
	GLuint GetTexture() {return texture;};
	void SetDocument(const char * document);
	void SetImage(const char * image);
	void SetDimension(float width, float height);
	void SetTexture(GLuint  _texture) { texture= _texture; };
	virtual void  Init();
	virtual void  onRender();
	virtual bool  onUpdate();
	virtual int	  GetMediaType() {return m_media_type;};

private:
 	GLuint m_vertex_buffers, m_texture_buffers;
	GLfloat vertices[12];
	GLfloat uvmap[8];
	char	imagefile[MAX_PATH];
	char	documentfile[MAX_PATH];
	GLfloat width, height;
	GLuint  texture;
	int		m_media_type;
};


class BrowserUI
{
public:
	BrowserUI();
	virtual ~BrowserUI() {};
	virtual void Init();
	virtual void onRender();
	virtual bool onUpdate();
	virtual bool AddThumbnail(const char *document, const char *image);
	virtual bool RemoveThumbnail(const char *document, const char *image);

	virtual void SelectNext();
	virtual void SelectPrev();
	virtual void SelectItem(size_t i);

	virtual unsigned int GetSelection() {return curr_selection;};
	virtual Thumbnail * GetThumbnail(int i) {return m_Thumbnail_List[i];};
	virtual unsigned int GetSize() {return m_Thumbnail_List.size();};
	void ParseDir(const char *path);

	void SetVisible(bool b) {visible=b;};
	bool IsVisible() {return visible;};

private:
	std::vector<Thumbnail *> m_Thumbnail_List;
	std::map<std::string, GLuint> m_TextureMap;
	unsigned int curr_selection;
	bool visible;
	bool m_MediaReady[DEVICE_MAX];
};


GLuint LoadTextureRAW( const char * filename, int wrap );
int DecodeImage(char * raw, const char * data);
bool IsExist( const char * filename);


#endif //BROWSERUI_H
