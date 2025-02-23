import{L as f,A as _,_ as A,a as p,b as C}from"./DrphBXC7Va.js";import{t as P,o as h,n as k,s as B,u as D,F as c,_ as y,a as g,b as v,c as S,d as T,e as b,U as V,S as Z}from"./ClRD3bPy6a.js";import{d as w,t as x,h as $,c as j,a as n,b as s,u as e,w as a,e as l,m as N,F as L,o as F,f as U}from"../e/DARLCvxo2G.js";const E={class:"flex items-center gap-2"},J={class:"flex justify-end gap-2"},O=w({__name:"TimeForm",setup(z){f.start();const o=P(h({ntpServer:B().min(1,{message:"NTP server must not be empty."}).max(255,{message:"NTP server must be at most 255 characters."}).regex(/^(?!:\/\/)([a-zA-Z0-9-_]+\.)*[a-zA-Z0-9][a-zA-Z0-9-_]+\.[a-zA-Z]{2,11}?$/,{message:"NTP server must be a valid DNS name."}),timesyncDelay:k().min(6e4,{message:"Time sync delay must be at least 60000."}).max(864e5,{message:"Time sync delay must be at most 86400000."})})),{handleSubmit:u,resetForm:r}=D({validationSchema:o});(async()=>{const i=await _.getTimeSettings();r({values:i}),f.end()})();const d=u(async i=>{x({title:"Saving changes..."}),await _.updateTimeSettings(i),r({values:i}),x({title:"Changes saved",description:$("pre",{class:"mt-2 w-[340px] rounded-md bg-slate-950 p-4"},$("code",{class:"text-white"},JSON.stringify(i,null,2)))})});return(i,t)=>(F(),j(L,null,[t[8]||(t[8]=n("div",null,[n("h3",{class:"text-lg font-medium"},"Time"),n("p",{class:"text-sm text-muted-foreground"}," Configure the time synchronization settings for your device ")],-1)),s(e(A)),n("form",{class:"space-y-8",onSubmit:t[0]||(t[0]=(...m)=>e(d)&&e(d)(...m))},[s(e(c),{name:"ntpServer"},{default:a(({componentField:m})=>[s(e(y),null,{default:a(()=>[s(e(g),null,{default:a(()=>t[1]||(t[1]=[l("NTP Server")])),_:1}),s(e(v),null,{default:a(()=>t[2]||(t[2]=[l(" The NTP server to use for time synchronization ")])),_:1}),s(e(S),null,{default:a(()=>[s(e(T),N({type:"string"},m),null,16)]),_:2},1024),s(e(b))]),_:2},1024)]),_:1}),s(e(c),{name:"timesyncDelay"},{default:a(({componentField:m})=>[s(e(y),null,{default:a(()=>[s(e(g),null,{default:a(()=>t[3]||(t[3]=[l("Timesync Delay")])),_:1}),s(e(v),null,{default:a(()=>t[4]||(t[4]=[l(" The delay between time synchronization intervals ")])),_:1}),n("div",E,[s(e(S),null,{default:a(()=>[s(e(T),N({type:"number"},m),null,16)]),_:2},1024),t[5]||(t[5]=n("div",null,"ms",-1))]),s(e(b))]),_:2},1024)]),_:1}),n("div",J,[s(e(p),{type:"button",variant:"outline",onClick:e(r)},{default:a(()=>[s(e(V),{class:"h-4 w-4"}),t[6]||(t[6]=l(" Reset Form "))]),_:1},8,["onClick"]),s(e(p),{type:"submit"},{default:a(()=>[s(e(Z),{class:"h-4 w-4"}),t[7]||(t[7]=l(" Save "))]),_:1})])],32)],64))}}),H=w({__name:"TimeSettings",setup(z){return(o,u)=>(F(),U(C,null,{default:a(()=>[s(O)]),_:1}))}});export{H as default};
