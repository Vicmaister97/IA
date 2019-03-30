Lenguaje de marcas como HTML, recupera archivos locales o a través de HTTP, hay un INTERPRETE (Navegador)

<?xml version="1.0" encoding="UTF-8"?>
<aiml version="2.0">
<!-- VisionBot v1.0 Agente conversacional para gestionar el contenido de las plataformas de series -->
<!-- Víctor García Carrera, victorgarcia@correo.ugr.es -->


<!-- Saludo -->
<category>
<pattern> Hola ^</pattern>
<template>
	<random>
		<li> VisionBot a su disposición </li>
		<li> Buenas!! Soy VisionBot y le ayudaré a gestionar sus series </li>
	</random>
</template>
</category>

<!-- Obtenemos nombre -->
<category>
<pattern> ^ me llamo * </pattern>
<template>
	<think> <set name="nombre"> <star index="2"/> </set> </think>
	Bonito nombre <get name="nombre"/>
</template>
</category>

<!-- COPIAR PARA OBTENER OTROS CAMPOS O HACER QUE MUESTRE TODOS LOS CAMPOS DE UNA SERIE !-->
<!-- Obtener sinopsis de una serie !-->
<category>
	<pattern> ^ de que va (la serie) <set>seriesnetflix</set> </pattern>
		<template>
			<srai> decode <map><name><star index="2"/></name> descripcion </map></srai> 
		</template>
</category>

<category>
	<pattern> ^ de que va (la serie) * </pattern>
		<template>
			Lo siento, la serie <star index="2"/> no está en el catálogo. Desea incluirla??
		</template>
</category>


<!-- Incluir un nuevo capítulo en una temporada de una serie. !-->
<!-- ^ Anade un nuevo capitulo de la temporada <set>number</set> de <set>seriesnetflix</set> -->
<category>
<pattern> ^ nuevo capitulo de la temporada <set>number</set> de <set>seriesnetflix</set> </pattern>  
<template>
		<!-- Verificamos que existe la temporada que se indica -->
		<think>
		<set var="existeSeason">
			<map><name><star index="3"/></name> temporada<star index="2"/> </map>
		</set>
		</think>

		<condition var="existeSeason">
			<li value="unknown"> Lo siento, no existe esa temporada. Desea incluir la temporada <star index="2"/> a la serie <star index="3"/> ?? </li>
			<li>
				<think>
				<set name="serie"> <star index="3"/></set>
				<set name="temporada"> <star index="2"/></set>
				<set name="topic">titcap</set>
				</think>
				Sin problema, dime el título del capítulo


			</li>
		</condition>

</template>
</category>

<category>
	<pattern> ^ nuevo capitulo de la temporada <set>number</set> de * </pattern>
		<template>
			Lo siento, la serie <star index="3"/> no está en el catálogo. Desea anadirla??
		</template>
</category>

<topic name ="titcap">

<!--  Pide el nombre de un capitulo    --> 
<category>  
	<pattern> * </pattern>  
	<template>
			<think>
			<set name="titulo"> <srai> code <star/> </srai> </set>
			<set name="topic">durcap</set>
			</think>
			Vale, dime la duración del capítulo
	</template>
</category>
</topic>

<topic name ="durcap">

<!-- Pide	la	duracion	de	un	capitulo. Anade el capitulo-->	
<category>	
	<pattern><set>number</set></pattern>	
	<template>
			<think>
				<set name="duracion"><star/></set>
				<set name="topic">unknown</set>
				</think>
				<!--	Operaciones	previas	a	la	insercion	en	el	map	-->	
				<think>
				<set var="listaprevia">
					<map><name><get name="serie"/></name> temporada<get name="temporada"/> </map>
				</set>

				<set var="addlista"><get name="titulo"/>@<get name="duracion"/>@</set>

				<set var="newlist">
					<srai> ADDITEM <get var="addlista"/> IN <get var="listaprevia"/> </srai>
				</set>

				<modifymap>
					<name><get name="serie"/></name>	
					<key>temporada<get name="temporada"/></key>	
					<value><get var="newlist"/></value>	
				</modifymap>
			</think>

			Este capitulo ya esta disponible en la plataforma
	</template>	
</category>
</topic>



<!-- Incluir el primer capítulo de una nueva temporada en una serie. !-->
<category>
<pattern> ^ nueva temporada <set>number</set> de <set>seriesnetflix</set> </pattern>  
<template>
		<!-- Verificamos que existe la temporada que se indica -->
		<think>
		<set var="existeSeason">
			<map><name><star index="3"/></name> temporada<star index="2"/> </map>
		</set>
		</think>

		<condition var="existeSeason">
			<!-- Si no existía esa temporada -->
			<li value="unknown">
				<think>
				<set name="serie"> <star index="3"/></set>
				<set name="temporada"> <star index="2"/></set>
				<set name="topic">titcap</set>
				</think>
				Okay, dime el título del capítulo.

			</li>
			<li>
				Lo siento, ya existe esa temporada.
			</li>
		</condition>

</template>
</category>

<category>
	<pattern> ^ nueva temporada <set>number</set> de * </pattern>
		<template>
			Lo siento, la serie <star index="3"/> no está en el catálogo. Desea anadirla??
		</template>
</category>

<topic name ="titcap">

<!--  Pide el nombre de un capitulo    --> 
<category>  
	<pattern> * </pattern>  
	<template>
			<think>
			<set name="titulo"> <srai> code <star/> </srai> </set>
			<set name="topic">durcap</set>
			</think>
			Vale, dime la duración del capítulo
	</template>
</category>
</topic>

<topic name ="durcap">

<!-- Pide	la	duracion	de	un	capitulo. Anade el capitulo-->	
<category>	
	<pattern><set>number</set></pattern>	
	<template>
				<set name="duracion"><star/></set>
				<set name="topic">unknown</set>
				<!--	Operaciones	previas	a	la	insercion	en	el	map	-->	

				<set var="newcap"><get name="titulo"/>@<get name="duracion"/></set>

				<addtomap>
					<name><get name="serie"/></name>	
					<key>temporada<get name="temporada"/></key>	
					<value><get var="newcap"/></value>	
				</addtomap>

			Esta temporada ya esta disponible en la plataforma
	</template>	
</category>
</topic>


<!-- Incluir una nueva serie en una de las plataformas. !-->




</aiml>

