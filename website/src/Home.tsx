import Navbar from "./Navbar.tsx";
import 'github-markdown-css/github-markdown.css';
import example from './assets/example4.png';

function Home() {
	return (
		<div>
			<Navbar location={"Home"} />
			<div className={"mt-16"}></div>
			<article className={"markdown-body p-4 min-h-[calc(100vh-4rem)] h-fit"}>
				<h1>Welcome to Open Render</h1>
				<p>Open Render is a single-header C++ 3D rendering engine that outputs raw RGBA values. It has various drawing functions and rotations. It is well documented and has a web demo.</p>
				<p><img src={example} alt={"Example of Open Render"} /></p>
				<h2>Quick Start</h2>
				<p>First, clone the repository.</p>
				<p>With URL</p>
				<pre><code className={"language-bash"}>git clone https://github.com/Bean91/Open-Render.git</code></pre>
				<p>Or with SSH</p>
				<pre><code className={"language-bash"}>git clone git@github.com:Bean91/Open-Render.git</code></pre>
				<p>Then, include the file in your code.</p>
				<pre><code className={"language-cpp"}>#include &quot;open_render.hpp&quot;</code></pre>
				<p>For extended documentation, please view the <a href={"docs/introduction.md"}>docs</a></p>
				<h2>Important Notes</h2>
				<p>This was made as a school semester project (v1 at least).</p>
				<p>If you would like to contribute, please look at the <a href={"https://github.com/Bean91/Open-Render"}><code>GitHub Repository</code></a></p>
			</article>
		</div>
	)
}

export default Home;