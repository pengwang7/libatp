#ifndef __ATP_API_H__
#define __ATP_API_H__

namespace atp {

#if defined(__clang__) || defined(__GNUC_STDC_INLINE__)
    #define INLINE_API(hdr, body) static hdr; static inline hdr body
#else
    #define INLINE_API(hdr, body) hdr; extern inline hdr body
#endif

#define PRIVATE_API(hdr, body) static hdr; static hdr body

#define DEFINE_PUBLIC_API(hdr) hdr;

#define PUBLIC_API(hdr, body) hdr body

} /* end namespace atp */

#endif /* __ATP_API_H__ */
