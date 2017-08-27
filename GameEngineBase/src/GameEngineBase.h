#pragma once

//
//	Game Engine Base Header File
//  Mark Diedericks - 2017
//

#include "Application/Application.h"
#include "Application/Window.h"

#include "Audio/Sound.h"
#include "Audio/SoundManager.h"

#include "Debug/DebugLayer.h"
#include "Debug/DebugMenu.h"
#include "Debug/DebugMenuAction.h"
#include "Debug/DebugMenuItem.h"

#include "Entity/Entity.h"
#include "Entity/Component/Component.h"
#include "Entity/Component/MeshComponent.h"
#include "Entity/Component/SpriteComponent.h"
#include "Entity/Component/TransformComponent.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Graphics/Renderer/Renderer3D.h"
#include "Graphics/Renderer/ForwardRenderer.h"
#include "Graphics/Renderer/DeferredRenderer.h"

#include "Graphics/Font/Font.h"
#include "Graphics/Font/FontManager.h"

#include "Graphics/GBuffer.h"
#include "Graphics/IRenderable.h"
#include "Graphics/Label.h"
#include "Graphics/Mask.h"

#include "Graphics/Material/Material.h"
#include "Graphics/Material/PBRMaterial.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshFactory.h"
#include "Graphics/Mesh/Model.h"

#include "Graphics/Renderable2D.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Scene.h"
#include "Graphics/Sprite.h"

#include "Graphics/Texture/TextureManager.h"

#include "Graphics/Camera/Camera.h"
#include "Graphics/Camera/FPSCamera.h"
#include "Graphics/Camera/MayaCamera.h"

#include "Graphics/Layer/Group.h"
#include "Graphics/Layer/Layer.h"
#include "Graphics/Layer/Layer2D.h"
#include "Graphics/Layer/Layer3D.h"

#include "Graphics/Shader/Shader.h"
#include "Graphics/Shader/ShaderFactory.h"
#include "Graphics/Shader/ShaderManager.h"

#include "Backend/API/APIShaderUniform.h"

#include "Graphics/Texture/Texture.h"
#include "Graphics/Texture/Texture2D.h"
#include "Graphics/Texture/TextureCube.h"
#include "Graphics/Texture/TextureDepth.h"

#include "Graphics/Framebuffer/Framebuffer.h"
#include "Graphics/Framebuffer/Framebuffer2D.h"
#include "Graphics/Framebuffer/FramebufferDepth.h"

#include "Math/Shapes/AABB.h"

#include "Math/Matrix4f.h"
#include "Math/Utils.h"
#include "Math/Quaternion.h"
#include "Math/Vector2f.h"
#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

#include "Utils/ImageUtil.h"
#include "Utils/TimeUtil.h"

#include "System/FileSystem.h"
#include "System/Memory.h"
#include "System/MemoryManager.h"
#include "System/VirtualFileSystem.h"

#include "CustomString.h"
#include "Types.h"


