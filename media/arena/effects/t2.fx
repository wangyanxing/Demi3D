<?xml version="1.0"?>
<Effects
>
	<ParticleSystem
		name="Fx_repeater1"
	>
		<Element
			name="Element_0"
		>
			<Material
			>
				<shader
					name="vertex"
				>basic_v</shader>
				<shader
					name="pixel"
				>basic_p</shader>
				<depthWrite
				>false</depthWrite>
				<blendMode
				>ADD</blendMode>
				<ambient
				>0 0 0 1</ambient>
				<shininess
				>16</shininess>
				<flag
				>USE_COLOR,USE_MAP</flag>
				<variables
				>
					<sampler2D
					>
						<texture
							name="map"
						>glow_01.dds</texture>
					</sampler2D>
				</variables>
			</Material>
			<Renderer
				type="Billboard"
			 />
			<Emitter
				type="Box"
				name="Box_0"
				emissionRate="fixed|30"
				totalTimeToLive="random|1,2"
				velocity="fixed|20"
				allDimensions="random|20,40"
				position="0 165 5"
				boxWidth="50"
				boxHeight="70"
				boxDepth="50"
			 />
			<Controller
				type="Color"
				name="Color_0"
				times="0,0.5,1"
				colors="0 0 0 1,0.25 1 0.5 1,0 0 0 1"
			 />
		</Element>
		<Element
			name="Element_1"
		>
			<Material
			>
				<shader
					name="vertex"
				>basic_v</shader>
				<shader
					name="pixel"
				>basic_p</shader>
				<depthWrite
				>false</depthWrite>
				<blendMode
				>ADD</blendMode>
				<ambient
				>0 0 0 1</ambient>
				<shininess
				>16</shininess>
				<flag
				>USE_COLOR,USE_MAP</flag>
				<variables
				>
					<sampler2D
					>
						<texture
							name="map"
							manualLoader="true"
						>world/props/plants/lillys/color</texture>
					</sampler2D>
				</variables>
			</Material>
			<Renderer
				type="Billboard"
			 />
			<Emitter
				type="Box"
				name="Box_1"
				emissionRate="fixed|6"
				totalTimeToLive="random|1,2"
				velocity="fixed|20"
				allDimensions="random|20,40"
				position="0 165 5"
				boxWidth="50"
				boxHeight="70"
				boxDepth="50"
			 />
			<Controller
				type="Color"
				name="Color_1"
				times="0,0.5,1"
				colors="0 0 0 1,0.25 1 0.5 1,0 0 0 1"
			 />
			<Controller
				type="TextureRotator"
				name="TextureRotator_2"
				ownRotationSpeed="true"
				rotation="random|0,300"
				rotationSpeed="random|-100,100"
			 />
		</Element>
	</ParticleSystem>
</Effects>
