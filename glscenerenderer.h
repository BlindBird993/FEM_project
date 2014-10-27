#ifndef __GLSCENERENDERER_H__
#define __GLSCENERENDERER_H__

#include <QQuickItem>


#include "gmlibwrapper.h"

// gmlib
#include <opengl/gmtexture>
#include <opengl/gmprogram>
#include <opengl/shaders/gmvertexshader.h>
#include <opengl/shaders/gmfragmentshader.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <scene/render/rendertargets/gmtexturerendertarget.h>

// qt
#include <QOpenGLShaderProgram>
#include <QDebug>


// stl
#include <memory>
#include <mutex>


namespace Private {




  // Invariants
  // Name must be valid!
  class Renderer: public QObject {
    Q_OBJECT
  public:
    Renderer( const std::string& name );

    void      setName( const std::string& name );

  public slots:
    void      paint();
    void      setViewport( const QRectF& rect );
    void      setViewport( int x, int y, int w, int h );

  private:
    QRectF                          _viewport;

    GMlib::GL::VertexBufferObject   _vbo;
    std::string                     _name;

    GMlib::GL::Program              _prog;
    GMlib::GL::VertexShader         _vs;
    GMlib::GL::FragmentShader       _fs;

  }; // END namespace Renderer

}


class GLSceneRenderer : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QString name     READ getTexName WRITE setTexName)
  Q_PROPERTY(bool    paused   READ isPaused   WRITE setPaused)


public:
  explicit GLSceneRenderer();

  const QString&        getTexName() const;
  void                  setTexName( const QString& tex_name );

  bool                  isPaused() const;
  void                  setPaused( bool paused );

private:
  std::unique_ptr<Private::Renderer>      _renderer;
  QString                                 _name;
  bool                                    _paused;

signals:
  void                  signViewportChanged( const QString& name, const QRectF& size );
  void                  signMousePressed( const QString& name, QMouseEvent* event );
  void                  signKeyPressed( const QString& name, QKeyEvent* event );

public slots:
  void                  sync();
  void                  cleanup();

protected slots:
  void                  itemChange(ItemChange change, const ItemChangeData& value ) override;
  QSGNode*              updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  void                  mousePressEvent(QMouseEvent *event) override;
  void                  keyPressEvent(QKeyEvent *event) override;

private slots:
  void                  handleWindowChanged( QQuickWindow * window );


};



#endif // __GLSCENERENDERER2_H__
