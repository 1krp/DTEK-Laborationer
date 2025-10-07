#!/usr/bin/env bash
set -euo pipefail

IN_DIR="./graphics/raws"
OUT_HDR="cardPxArrays.h"
GUARD="CARD_PX_ARRAYS_H"

# Header prologue
cat > "$OUT_HDR" <<EOF
/* Auto-generated from $IN_DIR on $(date) */
#ifndef $GUARD
#define $GUARD
#ifdef __cplusplus
extern "C" {
#endif

/* Each image is emitted as:
   static const unsigned char <name>[] = { ... };
   static const unsigned int  <name>_len = <N>;
*/
EOF

shopt -s nullglob
for f in "$IN_DIR"/*.raw; do
  base=$(basename "$f")
  name="${base%.*}"

  # sanitize to valid C identifier: non-alnum -> _, and prefix if starting with digit
  cname=$(echo "$name" | tr -c '[:alnum:]' '_' )
  case "$cname" in
    [0-9]* ) cname="img_${cname}";;
  esac

  echo "Embedding $f as $cname"

  # Emit as static const so it’s safe to include in multiple TUs
  xxd -i -n "$cname" "$f" \
  | sed -e 's/^unsigned char/static const unsigned char/' \
        -e 's/^unsigned int/static const unsigned int/' \
  >> "$OUT_HDR"
  echo >> "$OUT_HDR"
done

# Footer
cat >> "$OUT_HDR" <<EOF
#ifdef __cplusplus
}
#endif
#endif /* $GUARD */
EOF

echo "Done → $OUT_HDR"