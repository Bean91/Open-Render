import React, {useEffect, useRef, useState} from "react";
import { useParams } from "react-router";
import Markdown from 'react-markdown';
import 'github-markdown-css/github-markdown.css';
import remarkGfm from 'remark-gfm';
import { Prism as SyntaxHighlighter } from "react-syntax-highlighter";
import { oneDark } from "react-syntax-highlighter/dist/cjs/styles/prism";
import '../style.css';

interface docs {
	sec: boolean,
	ite: boolean
};

const components = {
  code({ node, inline, className, children, ...props }) {
    const match = /language-(\w+)/.exec(className || '');
    if (!inline && match) {
      return (
        <SyntaxHighlighter
          style={oneDark}
          language={match[1]}
          PreTag="div"
          {...props}
        >
          {String(children).replace(/\n$/, '')}
        </SyntaxHighlighter>
      );
    }
    return (
      <code className={className} {...props}>
        {children}
      </code>
    );
  },
};

const Sidebar: React.FC<docs> = ({sec, ite}) => {
	const sidebar = useRef<HTMLDivElement | null>(null);
	const { section, item } = useParams();
	const [content, setContent] = useState<string>("");
	const [docsList, setDocsList] = useState<string[]>([""]);

	useEffect(() => {
		console.log(section);
		console.log(item);
		fetch("http://localhost:8080/docs/list", {
			"method": "GET",
		})
		.then(response => response.json())
		.then(data => {
			setDocsList(data);
		});
		if (sec && !ite) {
			console.log("ai");
			fetch(`http://localhost:8080/docs/AIUsage`, {
			    "method": "GET"
			}).then(response => response.text()).then(data => {setContent(data);});
		} else if (!sec && !ite) {
			console.log("root");
			fetch(`http://localhost:8080/docs/introduction`, {
			    "method": "GET"
			}).then(response => response.text()).then(data => {setContent(data);});
		} else if (sec && ite) {
 			fetch(`http://localhost:8080/docs/${section}/${item}`, {
 			    "method": "GET"
 			}).then(response => response.text()).then(data => {setContent(data);});
 		}
	}, [section, item, sec, ite]);

	useEffect(() => {
		setContent(content.replace(".md", ""));
	}, [content])

	useEffect(() => {
		const classes = "cursor-pointer p-1 rounded-sm"
		if(sidebar.current) {
			if (!sec && !ite) {
				sidebar.current.innerHTML = `<p class="${classes} bg-gray-700"><a>Introduction</a></p><p class="${classes}"><a href="/docs/aiusage">AI Usage</a></p>`;
			} else if (sec && !ite) {
				sidebar.current.innerHTML = `<p class="${classes}"><a href="/docs">Introduction</a></p><p class="${classes} bg-gray-700"><a>AI Usage</a></p>`;
			} else {
				sidebar.current.innerHTML = `<p class="${classes}"><a href="/docs">Introduction</a></p><p class="${classes}"><a href="/docs/aiusage">AI Usage</a></p>`;
			}

		}
	}, [docsList, section, item, sec, ite, sidebar])

	return (
		<div className={"mt-16 grid grid-cols-5"}>
			<div ref={sidebar} className={"col-span-1 p-2 m-1 mr-0 bg-gray-600 text-white rounded-md"}></div>
			<article className={"markdown-body p-4 col-span-4 rounded-md"} style={{margin: "calc(var(--spacing))"}}>
				<Markdown
					remarkPlugins={[remarkGfm]}
					components={components}
				>{content}</Markdown>
			</article>
		</div>
	)
}

export default Sidebar;