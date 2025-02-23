import{L as f,A as p,_ as B,a as _,b as N}from"./DrphBXC7Va.js";import{t as R,o as T,n as c,u as V,_ as y,a as g,b,c as S,d as x,e as w,F as h,U as W,S as j}from"./ClRD3bPy6a.js";import{d as $,t as v,h as k,c as q,o as D,a as l,b as t,u as e,w as a,e as i,m as F,F as A,f as H}from"../e/DARLCvxo2G.js";const L={class:"flex items-center gap-2"},U={class:"flex items-center gap-2"},E={class:"flex justify-end gap-2"},J=$({__name:"DisplayForm",setup(C){f.start();const r=R(T({refreshDelay:c().min(100,{message:"Refresh delay must be at least 100ms."}).max(1e4,{message:"Refresh delay must be at most 10,000ms."}),wifiCheckDelay:c().min(100,{message:"Wifi check delay must be at least 100ms."}).max(6e4,{message:"Wifi check delay must be at most 60,000ms."})})),{handleSubmit:u,resetForm:o}=V({validationSchema:r});(async()=>{const n=await p.getDisplaySettings();o({values:n}),f.end()})();const d=u(async n=>{v({title:"Saving changes..."}),await p.updateDisplaySettings(n),o({values:n}),v({title:"Changes saved",description:k("pre",{class:"mt-2 w-[340px] rounded-md bg-slate-950 p-4"},k("code",{class:"text-white"},JSON.stringify(n,null,2)))})});return(n,s)=>(D(),q(A,null,[s[9]||(s[9]=l("div",null,[l("h3",{class:"text-lg font-medium"},"Display"),l("p",{class:"text-sm text-muted-foreground"}," Manage your display settings and set refresh intervals ")],-1)),t(e(B)),l("form",{class:"space-y-8",onSubmit:s[0]||(s[0]=(...m)=>e(d)&&e(d)(...m))},[t(e(h),{name:"refreshDelay"},{default:a(({componentField:m})=>[t(e(y),null,{default:a(()=>[t(e(g),null,{default:a(()=>s[1]||(s[1]=[i("Refresh delay")])),_:1}),t(e(b),null,{default:a(()=>s[2]||(s[2]=[i(" How frequently the display will refresh ")])),_:1}),l("div",L,[t(e(S),null,{default:a(()=>[t(e(x),F({type:"number"},m),null,16)]),_:2},1024),s[3]||(s[3]=l("div",null,"ms",-1))]),t(e(w))]),_:2},1024)]),_:1}),t(e(h),{name:"wifiCheckDelay"},{default:a(({componentField:m})=>[t(e(y),null,{default:a(()=>[t(e(g),null,{default:a(()=>s[4]||(s[4]=[i("WiFi check delay")])),_:1}),t(e(b),null,{default:a(()=>s[5]||(s[5]=[i(" How frequently the WiFi connection and signal strength will be checked ")])),_:1}),l("div",U,[t(e(S),null,{default:a(()=>[t(e(x),F({type:"number"},m),null,16)]),_:2},1024),s[6]||(s[6]=l("div",null,"ms",-1))]),t(e(w))]),_:2},1024)]),_:1}),l("div",E,[t(e(_),{type:"button",variant:"outline",onClick:e(o)},{default:a(()=>[t(e(W),{class:"h-4 w-4"}),s[7]||(s[7]=i(" Reset Form "))]),_:1},8,["onClick"]),t(e(_),{type:"submit"},{default:a(()=>[t(e(j),{class:"h-4 w-4"}),s[8]||(s[8]=i(" Save "))]),_:1})])],32)],64))}}),z=$({__name:"DisplaySettings",setup(C){return(r,u)=>(D(),H(N,null,{default:a(()=>[t(J)]),_:1}))}});export{z as default};
