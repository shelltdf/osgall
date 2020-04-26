/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include <sys/process.h>
#include <sys/paths.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <cell/dbgfont.h>
#include "FWGLCamControlApplication.h"

#include "BrowserUI.h"

const char * DEFAULT_IMAGE = 0;

const char * MEDIA_MOUNT_PATH[DEVICE_MAX] =
{
"/dev_ms",
"/dev_usb000",
"/dev_usb001",
"/dev_usb002",
"/dev_usb003",
"/dev_usb004",
"/dev_usb005",
"/dev_usb006",
};

Thumbnail::Thumbnail()
{
	width = height = THUMBNAIL_SIZE;
	SetPosition(0, 0, 0);
	uvmap[0] = 0.0; uvmap[1] = 0.0; 
	uvmap[2] = 1.0; uvmap[3] = 0.0; 
	uvmap[4] = 1.0; uvmap[5] = 1.0; 
	uvmap[6] = 0.0; uvmap[7] = 1.0; 

	texture = 0;

	glGenBuffers(1, &m_vertex_buffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffers );
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
Thumbnail::~Thumbnail()
{
	glDeleteBuffers(1, &m_vertex_buffers);
	glDeleteBuffers(1, &m_texture_buffers);
}

void Thumbnail::SetDimension(float _width, float _height)
{
	width = _width;
	height = _height;
	vertices[3] = vertices[0]+width;
	vertices[6] = vertices[0]+width; 
	vertices[7] = vertices[1]+height; 
	vertices[10] = vertices[1]+height; 
}

void Thumbnail::SetPosition(float x, float y, float z)
{
	vertices[0] = x; vertices[1] = y; vertices[2] = z;
	vertices[3] = x+width; vertices[4] = y; vertices[5] = z;
	vertices[6] = x+width; vertices[7] = y+height; vertices[8] = z;
	vertices[9] = x; vertices[10] = y+height; vertices[11] = z;
}

void Thumbnail::MovePosition(float x, float y, float z)
{
	for (int i=0; i<12; i=i+3)
	{
		vertices[i] = vertices[i] + x;
		vertices[i+1] = vertices[i+1] + y;
		vertices[i+2] = vertices[i+2] + z;
	}
}

void Thumbnail::Init()
{
}

void Thumbnail::onRender()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffers);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, texture );

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffers);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvmap), uvmap, GL_STATIC_DRAW);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	glDisableClientState(GL_NORMAL_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4); 
}

bool Thumbnail::onUpdate()
{
	return true;
}

void Thumbnail::SetDocument(const char * document)
{
	if (document)
	{
		strncpy(documentfile, document, MAX_PATH);
		for (int i=0; i<DEVICE_MAX; i++)
		{
			if (strstr(document, MEDIA_MOUNT_PATH[i]) != NULL)
			{
				m_media_type = i;
				return;
			}
		}
	}
}

void Thumbnail::SetImage(const char * image)
{
	if (image)
	{
		strncpy(imagefile, image, MAX_PATH);
		glGenBuffers(1, &m_texture_buffers);
		glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffers );
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvmap), uvmap, GL_STATIC_DRAW);
	}	
}

/*==========================*/
BrowserUI::BrowserUI() 
{
	curr_selection=0;visible=false;
	for(int i=0; i<DEVICE_MAX; i++)
	{	
		m_MediaReady[i] = false;
	}
}

void BrowserUI::ParseDir(const char *path)
{
	int err;
    DIR *  dir = 0;
    struct dirent * dent;

	dir = opendir(path);
	if (dir == 0) {
	    printf("can't open %s\n", path);
		return;
	}

	char pathname[MAX_PATH];
	char t_pathname[MAX_PATH];
    while (1) {

        dent = readdir(dir);
		if (dent == 0)
			break;

		if (dent->d_type == DT_DIR)
		{
			if (strcmp(dent->d_name, ".")==0 || strcmp(dent->d_name, "..")==0)
				continue;
			sprintf(pathname, "%s/%s", path, dent->d_name);
			ParseDir(pathname);
		} else if (dent->d_type == DT_REG)
		{
			size_t filelen = strlen(dent->d_name);
			if ( ((dent->d_name[filelen-4] == '.')) &&
				 ((dent->d_name[filelen-3] == 'd') || (dent->d_name[filelen-3] == 'D')) &&
				 ((dent->d_name[filelen-2] == 'a') || (dent->d_name[filelen-2] == 'A')) &&
				 ((dent->d_name[filelen-1] == 'e') || (dent->d_name[filelen-1] == 'E'))
				 )
			{
				sprintf(pathname, "%s/%s", path, dent->d_name);
				printf("%s is a collada document\n", pathname);
				sprintf(t_pathname, "%s/%s.raw", path, dent->d_name);
				AddThumbnail(pathname, t_pathname);
			}
		}
    } 

    err = closedir(dir);
    API_ERROR(err);
}

void BrowserUI::Init()
{
	// make default texture
	DEFAULT_IMAGE = HDD_DEFAULT_IMAGE;
	int ret = LoadTextureRAW(DEFAULT_IMAGE, 0);
	if (ret!=-1)
		m_TextureMap[DEFAULT_IMAGE] = ret;
	else
	{
	DEFAULT_IMAGE = APP_HOME_DEFAULT_IMAGE;
	int ret = LoadTextureRAW(DEFAULT_IMAGE, 0);
	if (ret!=-1)
		m_TextureMap[DEFAULT_IMAGE] = ret;
		
	}
}
void BrowserUI::onRender()
{
	if (visible==false) return;
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glMatrixMode( GL_MODELVIEW ); 
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK ); 
	glEnable( GL_TEXTURE_2D );
	glLoadIdentity();
	glTranslatef(-2.0, 0.0, -5.0);

	for (size_t i=0; i<m_Thumbnail_List.size(); i++)
	{
		m_Thumbnail_List[i]->onRender();
	}
	glEnable(GL_LIGHTING);
    glPopMatrix();
}

bool BrowserUI::onUpdate()
{
	int err=0;
    struct stat status;
	for (int i=0; i<DEVICE_MAX; i++)
	{
		err = stat(MEDIA_MOUNT_PATH[i], &status);
        if (err == CELL_OK)
		{			// memory inserted
			if (m_MediaReady[i] == false)
			{
		        printf("Mount %s\n", MEDIA_MOUNT_PATH[i]);
				ParseDir(MEDIA_MOUNT_PATH[i]);
				m_MediaReady[i] = true;
			}
		} else {	// memory ejected
			if (m_MediaReady[i] == true)
			{
		        printf("Umount %s\n", MEDIA_MOUNT_PATH[i]);
				SelectItem(0);
				for (int j=m_Thumbnail_List.size()-1; j>=0; j--)
				{
					if (m_Thumbnail_List[j]->GetMediaType()==i)
					{
						RemoveThumbnail(m_Thumbnail_List[j]->GetDocument(), m_Thumbnail_List[j]->GetImage());
						delete m_Thumbnail_List[j];
						std::vector<Thumbnail *>::iterator it = m_Thumbnail_List.begin() + j;
						m_Thumbnail_List.erase(it);
					}
				}
				m_MediaReady[i] = false;
			}
		}
	}
	return true;
}


bool BrowserUI::AddThumbnail(const char *document, const char *image)
{
	if (document == 0) return false;
	Thumbnail * tn = new Thumbnail();
	if (tn == 0) return false;
	tn->SetDocument(document);
	if (image) {
		if (IsExist(image))
		{		// texture file exist
			int ret = LoadTextureRAW(image, 0);
			if (ret != -1)
			{		// new texture created
				m_TextureMap[image] = ret;
				tn->SetImage(image);
				tn->SetTexture(ret);
			} else { // use default texture
				tn->SetImage(DEFAULT_IMAGE);
				tn->SetTexture(m_TextureMap[DEFAULT_IMAGE]);				
			}
		} else { //image file not exist, use default texture
				tn->SetImage(DEFAULT_IMAGE);
				tn->SetTexture(m_TextureMap[DEFAULT_IMAGE]);				
		}
	}
	tn->SetPosition(0.0, (-0.5) * m_Thumbnail_List.size(), 0);
	m_Thumbnail_List.push_back(tn);
	if (curr_selection==0 && m_Thumbnail_List.size()==1) {
		tn->MovePosition(0.0, -THUMBNAIL_HALFSIZE, 0);
		tn->SetDimension(THUMBNAIL_SIZE *2, THUMBNAIL_SIZE *2);
	}
	return true;
}

bool BrowserUI::RemoveThumbnail(const char *document, const char *image)
{
	(void) document;
	(void) image;
	return true;
};

void BrowserUI::SelectItem(size_t iItem)
{
	if (iItem == m_Thumbnail_List.size()-1) return;
	m_Thumbnail_List[curr_selection]->SetDimension(THUMBNAIL_SIZE, THUMBNAIL_SIZE);
	curr_selection = iItem;

	float top_pos = curr_selection * MOVE_SPACE;
	for (size_t i=0; i<m_Thumbnail_List.size(); i++)
	{
		m_Thumbnail_List[i]->SetPosition(0.0,  top_pos - MOVE_SPACE * i, 0);
	}
	m_Thumbnail_List[curr_selection]->SetDimension(THUMBNAIL_SIZE * 2, THUMBNAIL_SIZE * 2);	
}

void BrowserUI::SelectNext()
{
	if (curr_selection == m_Thumbnail_List.size()-1) return;
	m_Thumbnail_List[curr_selection]->MovePosition(0, THUMBNAIL_HALFSIZE, 0);
	m_Thumbnail_List[curr_selection]->SetDimension(THUMBNAIL_SIZE, THUMBNAIL_SIZE);
	curr_selection++;

	for (size_t i=0; i<m_Thumbnail_List.size(); i++)
	{
		m_Thumbnail_List[i]->MovePosition(0, MOVE_SPACE, 0);
	}
	m_Thumbnail_List[curr_selection]->MovePosition(0, -THUMBNAIL_HALFSIZE, 0);
	m_Thumbnail_List[curr_selection]->SetDimension(THUMBNAIL_SIZE * 2, THUMBNAIL_SIZE * 2);
}

void BrowserUI::SelectPrev()
{
	if (curr_selection == 0) return;
	m_Thumbnail_List[curr_selection]->MovePosition(0, THUMBNAIL_HALFSIZE, 0);
	m_Thumbnail_List[curr_selection]->SetDimension(THUMBNAIL_SIZE, THUMBNAIL_SIZE);
	curr_selection--;

	for (size_t i=0; i<m_Thumbnail_List.size(); i++)
	{
		m_Thumbnail_List[i]->MovePosition(0, - MOVE_SPACE, 0);
	}
	m_Thumbnail_List[curr_selection]->MovePosition(0, -THUMBNAIL_HALFSIZE, 0);
	m_Thumbnail_List[curr_selection]->SetDimension(THUMBNAIL_SIZE * 2, THUMBNAIL_SIZE * 2);
}

int DecodeImage(char * raw, const char * data)
{
	int cur_data = 0;
	int cur_raw = 0;
	for (int i=0; i<256; i++)
	{
		for (int j=0; j<256; j++)
		{
			cur_data = ((256*(255-i))+j)*3;
			cur_raw = ((256*i)+j)*4;
			raw[cur_raw+0] = data[cur_data+0];
			raw[cur_raw+1] = data[cur_data+1];
			raw[cur_raw+2] = data[cur_data+2];
			raw[cur_raw+3] = 0xFF;
		}
	}
	return 0;
}

bool IsExist( const char * filename)
{
    FILE * file = NULL;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return false;
	fclose(file);
	return true;
}
GLuint LoadTextureRAW( const char * filename, int wrap )
{
	int err = 0;
    GLuint texture;
    int width = 256;
    int height = 256;
    char * data = NULL;
	char * raw = NULL;
    FILE * file = NULL;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return -1;

    // allocate buffer
	data = new char[width * height * 3];
	raw = new char[width * height * 4];

	if (data == NULL) return -1;

    // read texture data
    err = fread( data, sizeof(char), width * height * 3, file );
	if (err != width * height * 3) return -1;

	DecodeImage(raw, data);

    fclose( file );

	// allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     wrap ? GL_REPEAT : GL_CLAMP );

    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height,
                       GL_RGBA, GL_UNSIGNED_BYTE, raw );

	delete [] data;
	delete [] raw;

    return texture;
}
